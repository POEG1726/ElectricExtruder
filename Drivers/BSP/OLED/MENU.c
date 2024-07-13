#include "MENU.h"
#include "Keys.h"
#include "stdlib.h"

volatile OLED_INTERFACE UI_Interface = OPERATION_INTERFACE;
uint8_t Current_Choice = 0;

void Menu_SetChoice(uint8_t Direction, uint8_t* pChoice)
{
    uint8_t choice = 0;
    switch (Direction)
    {
    case 0:
        if (*pChoice == 0)
        {
            choice = 2;
            *pChoice = 2;
        }
        else
        {
            choice = *pChoice - 1;
            *pChoice -= 1;
        }
        break;
    case 1:
        if (*pChoice == 2)
        {
            choice = 0;
            *pChoice = 0;
        }
        else
        {
            choice = *pChoice + 1;
            *pChoice += 1;
        }
        break;
    default:
        break;
    }

    switch (choice)
    {
    case 0:
        // OLED_ReverseArea(0, 0, 128, 8);
        OLED_ShowChar(0, 0, '*');
        OLED_ClearArea(0, 10, 6, 8);
        OLED_ClearArea(0, 20, 6, 8);
        break;
    case 1:
        OLED_ClearArea(0, 0, 6, 8);
        OLED_ShowChar(0, 10, '*');
        OLED_ClearArea(0, 20, 6, 8);
        break;
    case 2:
        OLED_ClearArea(0, 0, 6, 8);
        OLED_ClearArea(0, 10, 6, 8);
        OLED_ShowChar(0, 20, '*');
        break;
    default:
        *pChoice = 0;
        OLED_ShowChar(0, 0, '*');
        OLED_ClearArea(0, 10, 6, 8);
        OLED_ClearArea(0, 20, 6, 8);
        break;
    }
    OLED_Refresh();
}

void Menu_SwitchOptions()
{
    switch (Current_Choice)
    {
    case 0:
        OLED_ClearArea(80, 0, 48, 8);
        switch (UI_Interface)
        {
        case MOTOR_SETTINGS_INTERFACE:
            if (Current_Config->Motor == MOTOR_TYPE_N20)
            {
                OLED_ShowString(80, 0, "28BYJ48");
                Current_Config->Motor = MOTOR_TYPE_28BYJ48;
            }
            else if (Current_Config->Motor == MOTOR_TYPE_28BYJ48)
            {
                OLED_ShowString(80, 0, "N20");
                Current_Config->Motor = MOTOR_TYPE_N20;
            }
            break;

        case BATTERY_SETTINGS_INTERFACE:
            Config.Threshold_Volt += 0.1;
            if (Config.Threshold_Volt > 3.8 || Config.Threshold_Volt < 2.7) Config.Threshold_Volt = 2.7;
            OLED_ShowFloat(80, 0, Config.Threshold_Volt, 1, 1, 0);
            OLED_ShowString(99, 0, "V");
            break;
        case EXTRUSION_SETTINGS_INTERFACE:
            break;
        default:
            break;
        }
        break;
    case 1:
        OLED_ClearArea(80, 10, 48, 8);
        switch (UI_Interface)
        {
        case MOTOR_SETTINGS_INTERFACE:
            if (Current_Config->MotorControl == MOTOR_CONTROL_TYPE_MANUAL)
            {
                OLED_ShowString(80, 10, "Quant");
                Current_Config->MotorControl = MOTOR_CONTROL_TYPE_QUANTITATIVE;
            }
            else if (Current_Config->MotorControl == MOTOR_CONTROL_TYPE_QUANTITATIVE)
            {
                OLED_ShowString(80, 10, "Manual");
                Current_Config->MotorControl = MOTOR_CONTROL_TYPE_MANUAL;
            }
            break;
        case BATTERY_SETTINGS_INTERFACE:
            OLED_ShowString(93, 10, "Mins");
            Config.IdleTimeoutMinutes += 2;
            if (Config.IdleTimeoutMinutes > 60) Config.IdleTimeoutMinutes = 2;
            OLED_ShowNum(80, 10, Config.IdleTimeoutMinutes);
            break;
        default:
            break;
        }
        break;
    case 2:
        OLED_ClearArea(80, 20, 48, 8);
        switch (UI_Interface)
        {
        case MOTOR_SETTINGS_INTERFACE:
            Current_Config->MotorSpeed += 10;
            if (Current_Config->MotorSpeed > 80 || Current_Config->MotorSpeed < 40) Current_Config->MotorSpeed = 40;
            OLED_ShowNum(80, 20, Current_Config->MotorSpeed);
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }

    OLED_Refresh();
}

void Menu_Operation(void)
{
    UI_Interface = OPERATION_INTERFACE;
    OLED_Clear();
    OLED_ShowString(0, 0, "Volt:3.12V");
    OLED_ShowString(73, 0, (char*)Motor_Status);
    OLED_ShowString(0, 10, "ExtrAmt: 3ml");
    OLED_Refresh();
}

void Menu_Settings(uint8_t Entry)
{
    UI_Interface = SETTINGS_INTERFACE;
    Current_Choice = 0;
    if (Entry != 0) {};/* Returned to this func */
    OLED_Clear();
    OLED_ShowString(8, 0, "Motor Settings");
    OLED_ShowString(8, 10, "Battery Settings");
    OLED_ShowString(8, 20, "Extrusion Settings");
    OLED_ShowChar(0, 0, '*');
    OLED_Refresh();
}

void Menu_MotorSettings(void)
{
    UI_Interface = MOTOR_SETTINGS_INTERFACE;

    OLED_Clear();
    OLED_ShowString(8, 0, "Motor:");
    if (Current_Config->Motor == MOTOR_TYPE_N20) OLED_ShowString(80, 0, "N20");
    else OLED_ShowString(80, 0, "28BYJ48");

    OLED_ShowString(8, 10, "Control:");
    if (Current_Config->MotorControl == MOTOR_CONTROL_TYPE_MANUAL) OLED_ShowString(80, 10, "Manual");
    else OLED_ShowString(80, 10, "Quant");

    OLED_ShowString(8, 20, "Speed:");
    OLED_ShowNum(80, 20, Current_Config->MotorSpeed);
    // OLED_ShowString(70, 20, "N20");

    OLED_ShowChar(0, 0, '*');

    OLED_Refresh();
}

void Menu_BatterySettings(void)
{
    UI_Interface = BATTERY_SETTINGS_INTERFACE;

    OLED_Clear();
    OLED_ShowString(8, 0, "TH Volt:");
    OLED_ShowFloat(80, 0, Config.Threshold_Volt, 1, 1, 0);
    OLED_ShowString(99, 0, "V");

    OLED_ShowString(8, 10, "IDLE Time:");
    OLED_ShowNum(80, 10, Config.IdleTimeoutMinutes);
    OLED_ShowString(93, 10, "Mins");

    OLED_ShowChar(0, 0, '*');

    OLED_Refresh();
}

void Menu_ExtrusionSettings(void)
{
    UI_Interface = EXTRUSION_SETTINGS_INTERFACE;

    OLED_Clear();
    OLED_ShowString(8, 0, "ExtrAmt:");
    OLED_ShowNum(80, 0, Config.LiquidAmount);

    OLED_ShowString(8, 10, "Height:");
    OLED_ShowNum(80, 10, Config.ContainerHeight);

    OLED_ShowChar(0, 0, '*');

    OLED_Refresh();
}

void Menu_GetInChildMenu(void)
{
    switch (Current_Choice)
    {
    case 0:
        Current_Choice = 0;
        Menu_MotorSettings();
        break;
    case 1:
        Current_Choice = 0;
        Menu_BatterySettings();
        break;
    case 2:
        Current_Choice = 0;
        Menu_ExtrusionSettings();
        break;
    default:
        break;
    }
}