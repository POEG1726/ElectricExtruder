#include "Utilities.h"
#include "MENU.h"
#include "tim.h"
#include "OLED.h"
#include "eeprom.h"
#include "usart.h"

#define KEY_UP_ID 0
#define KEY_FUNC_ID 1
#define KEY_DOWN_ID 2
#define IsFirstRunBitAddr 0x0000
#define Current_ConfigAddr 0x0001
#define BUFFER_SIZE (1 + 1 + sizeof(ExtruderConfig))

Key Key_Up, Key_Func, Key_Down;
char Motor_Status[8];
extern uint8_t Current_Choice;

void Transmit_Message(char* buf)
{
    HAL_UART_Transmit(&huart2, (const uint8_t*)buf, strlen(buf), 100);
}

void SetMotorStatus(const char* status)
{
    strncpy(Motor_Status, status, 8);
    OLED_ShowString(73, 0, Motor_Status);
    OLED_Refresh();
}

uint8_t ReadKeyGPIO(uint8_t button_id)
{
    switch (button_id)
    {
    case KEY_UP_ID:
        return HAL_GPIO_ReadPin(Key_Up_GPIO_Port, Key_Up_Pin);
        break;

    case KEY_FUNC_ID:
        return HAL_GPIO_ReadPin(Key_Func_GPIO_Port, Key_Func_Pin);
        break;

    case KEY_DOWN_ID:
        return HAL_GPIO_ReadPin(Key_Down_GPIO_Port, Key_Down_Pin);
        break;

    default:
        return 0;
        break;
    }
}

// void Key_PressDownHandler(void* btn)
// {

// }

void Key_SingleClickHandler(void* btn)
{
    // Transmit_Message("SingleClick Detected\n");
    Key* Tmp_Key = btn;
    switch (UI_Interface)
    {
    case OPERATION_INTERFACE:
        switch (Tmp_Key->Key_id)
        {
        case KEY_UP_ID:
            SetMotorStatus("Pushing");
            HAL_Delay(200);
            SetMotorStatus("Standby");
            break;
        case KEY_FUNC_ID:
            SetMotorStatus("Cleared");
            HAL_Delay(200);
            SetMotorStatus("Standby");
            break;
        case KEY_DOWN_ID:
            SetMotorStatus("Pulling");
            HAL_Delay(200);
            SetMotorStatus("Standby");
            break;
        default:
            break;
        }
        break;

    case SETTINGS_INTERFACE:
        switch (Tmp_Key->Key_id)
        {
        case KEY_UP_ID:
            Menu_SetChoice(0, &Current_Choice);
            break;
        case KEY_FUNC_ID:
            Menu_GetInChildMenu();
            break;
        case KEY_DOWN_ID:
            Menu_SetChoice(1, &Current_Choice);
            break;
        default:
            break;
        }
        break;

    case MOTOR_SETTINGS_INTERFACE:
        switch (Tmp_Key->Key_id)
        {
        case KEY_UP_ID:
            Menu_SetChoice(0, &Current_Choice);
            break;
        case KEY_FUNC_ID:
            Menu_SwitchOptions();
            break;
        case KEY_DOWN_ID:
            Menu_SetChoice(1, &Current_Choice);
            break;
        default:
            break;
        }
        break;
    case BATTERY_SETTINGS_INTERFACE:
        switch (Tmp_Key->Key_id)
        {
        case KEY_UP_ID:
            Menu_SetChoice(0, &Current_Choice);
            break;
        case KEY_FUNC_ID:
            Menu_SwitchOptions();
            break;
        case KEY_DOWN_ID:
            Menu_SetChoice(1, &Current_Choice);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void Key_LongPressHandler(void* btn)
{
    // Transmit_Message("LongPress Detected\n");
    Key* Tmp_Key = btn;
    if (Tmp_Key->Key_id == KEY_FUNC_ID)
    {
        switch (UI_Interface)
        {
        case SETTINGS_INTERFACE:
            Menu_Operation();
            break;
        case OPERATION_INTERFACE:
            Menu_Settings(0);
            break;
        default:
            Menu_Settings(1);
            break;
        };

    }
}

/* @note Init Config and Read */
uint8_t LoadConfig(ExtruderConfig* Data)
{
    uint8_t IsFirstRunBit = 0x00;

    EEPROM_ReadByte(IsFirstRunBitAddr, &IsFirstRunBit);

    if (IsFirstRunBit != 0xBB)
    {/* 0xBB,0x00,ConfigData1,ConfigData2,ExtruderConfig */
        EEPROM_WriteByte(IsFirstRunBitAddr, IsFirstRunBit);
        HAL_Delay(5);

        EEPROM_ReadByte(IsFirstRunBitAddr, &IsFirstRunBit);
        if (IsFirstRunBit != 0xBB) return 1;

        uint8_t EEPROM_Buffer[BUFFER_SIZE];

        ExtruderConfig tmp_Config = {
        .MotorSettings_1.Motor = MOTOR_TYPE_28BYJ48,
        .MotorSettings_1.MotorControl = MOTOR_CONTROL_TYPE_QUANTITATIVE,
        .MotorSettings_1.MotorPosition = 0,
        .MotorSettings_1.MotorSpeed = 100,

        .MotorSettings_2.Motor = MOTOR_TYPE_N20,
        .MotorSettings_2.MotorControl = MOTOR_CONTROL_TYPE_QUANTITATIVE,
        .MotorSettings_2.MotorPosition = 0,
        .MotorSettings_2.MotorSpeed = 100
        };

        EEPROM_Buffer[0] = 0xBB;
        EEPROM_Buffer[1] = 0x01;
        memcpy(&EEPROM_Buffer[2], &tmp_Config, sizeof(ExtruderConfig));

        uint8_t size = sizeof(EEPROM_Buffer);
        EEPROM_WriteBuffer(0x0000, EEPROM_Buffer, sizeof(EEPROM_Buffer));
    }

    uint8_t tmp;
    EEPROM_ReadByte(0x0002, &tmp);
    switch (tmp)
    {
    case 0x01:
        EEPROM_ReadBuffer(0x0002, (uint8_t*)Data, sizeof(ExtruderConfig));
        Current_Config = &(Config.MotorSettings_1);
        break;
    case 0x02:
        EEPROM_ReadBuffer(sizeof(ExtruderConfig) + 2, (uint8_t*)Data, sizeof(ExtruderConfig));
        Current_Config = &(Config.MotorSettings_2);
        break;
    default:
        return 1;
        break;
    }
    return 0;
}

void SaveConfig(ExtruderConfig* Data)
{
    EEPROM_WriteBuffer(0x0002, (uint8_t*)Data, sizeof(ExtruderConfig));
}

void Set_Current_Config(uint8_t ConfigNum)
{
    if (ConfigNum == 0x01) Current_Config = &(Config.MotorSettings_1);
    else if(ConfigNum==0x02) Current_Config = &(Config.MotorSettings_2);
}

void Keys_Init(void)
{
    Key_init(&Key_Up, ReadKeyGPIO, 1, KEY_UP_ID);
    Key_attach(&Key_Up, SINGLE_CLICK, Key_SingleClickHandler);
    Key_start(&Key_Up);

    Key_init(&Key_Func, ReadKeyGPIO, 0, KEY_FUNC_ID);
    Key_attach(&Key_Func, SINGLE_CLICK, Key_SingleClickHandler);
    Key_attach(&Key_Func, LONG_PRESS_START, Key_LongPressHandler);
    Key_start(&Key_Func);

    Key_init(&Key_Down, ReadKeyGPIO, 0, KEY_DOWN_ID);
    Key_attach(&Key_Down, SINGLE_CLICK, Key_SingleClickHandler);
    Key_start(&Key_Down);

    HAL_TIM_Base_Start_IT(&htim14);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim == (&htim14))
    {
        // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        Key_ticks();
    }
}