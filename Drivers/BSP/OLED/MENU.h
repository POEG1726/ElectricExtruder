/*
    MENU.h
    Created on Jul 08, 2024
    Author: @POEG1726
*/

#ifndef _MENU_H_
#define _MENU_H_

#include "stm32g0xx.h"
#include "OLED.h"
#include "Utilities.h"

void Menu_SetChoice(uint8_t choice, uint8_t* pChoice);
void Menu_GetInChildMenu(void);
void Menu_Operation(void);
void Menu_Settings(uint8_t Entry);
void Menu_SwitchOptions();

extern ExtruderConfig Config;
extern uint8_t Child_Choice;
extern MotorSettings* Current_Config;
extern char Motor_Status[8];
extern volatile OLED_INTERFACE UI_Interface;

#endif // _MENU_H_