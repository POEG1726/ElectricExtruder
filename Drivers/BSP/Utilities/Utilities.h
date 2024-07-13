/*
    Utilities.h
    Created on Jul 08, 2024
    Author: @POEG1726
*/

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "stm32g0xx.h"
#include "Keys.h"

typedef enum
{
    FIRST_RUN_FALSE = 0,
    FIRST_RUN_TRUE = 1
} FirstRunStatus;

typedef enum
{
    MOTOR_TYPE_N20 = 0,
    MOTOR_TYPE_28BYJ48 = 1
} MotorType;

typedef enum
{
    MOTOR_CONTROL_TYPE_QUANTITATIVE = 0,
    MOTOR_CONTROL_TYPE_MANUAL = 1
}MotorControl;

typedef struct
{
    MotorType Motor;
    MotorControl MotorControl;
    uint8_t MotorSpeed;
    uint16_t MotorPosition;
}MotorSettings;

typedef enum
{
    MOTOR_FORWARD = 0,
    MOTOR_REVERSE = 1
}MotorDirection;

typedef struct
{
    MotorSettings MotorSettings_1;/* 28BYJ48 */
    MotorSettings MotorSettings_2;/* N20 */
    float Threshold_Volt;
    uint8_t IdleTimeoutMinutes;
    uint8_t LiquidAmount;
    uint8_t ContainerHeight;
} ExtruderConfig;

typedef enum
{
    OPERATION_INTERFACE = 0,
    SETTINGS_INTERFACE,
    MOTOR_SETTINGS_INTERFACE,
    BATTERY_SETTINGS_INTERFACE,
    EXTRUSION_SETTINGS_INTERFACE,
}OLED_INTERFACE;

void Keys_Init(void);
void Transmit_Message(char* buf);
void SetMotorStatus(const char* status);
uint8_t LoadConfig(ExtruderConfig* Data);
void SaveConfig(ExtruderConfig* Data);
void Set_Current_Config(uint8_t ConfigNum);

extern MotorSettings* Current_Config;
extern Key Key_Up, Key_Func, Key_Down;
extern volatile OLED_INTERFACE UI_Interface;
extern char Motor_Status[8];
extern uint8_t Child_Choice;
extern ExtruderConfig Config;

#define ConfigSelection (*selectedMotorSettings)

#endif // _UTILITIES_H_