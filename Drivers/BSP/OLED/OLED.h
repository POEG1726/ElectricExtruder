/*
    OLED.h
    Created on Jul 07, 2024
    Author: @POEG1726
*/

#ifndef _OLED_H_
#define _OLED_H_
#include "stm32g0xx.h"

#define OLED_I2CTimeout HAL_MAX_DELAY

extern uint8_t OLED_RAM[8][128];

void OLED_Init(void);

void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_SetCursor(uint8_t XValue, uint8_t Page);
uint32_t OLED_Pow(uint8_t X, uint8_t Y);
uint8_t OLED_GetIntLen(uint32_t Number);
void OLED_Clear(void);
void OLED_ClearArea(uint8_t XValue, uint8_t YValue, uint8_t Width, uint8_t Height);
void OLED_Refresh(void);
void OLED_RefreshArea(uint8_t XValue, uint8_t YValue, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(uint8_t XValue, uint8_t YValue, uint8_t Width, uint8_t Height);
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t* Image);
void OLED_ShowChar(uint8_t XValue, uint8_t YValue, char data);
void OLED_ShowString(uint8_t XValue, uint8_t YValue, char* str);
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number);
void OLED_ShowFloat(uint8_t X, uint8_t Y, float Number, uint8_t IntLength, uint8_t FraLength, uint8_t ShowSign);

#endif // _OLED_H_