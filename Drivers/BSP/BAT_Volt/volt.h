/*
    volt.h
    Created on Jun 29, 2024 
    Author: @POEG1726
*/

#ifndef _VOLT_H_
#define _VOLT_H_

#include "stm32g0xx.h"

extern uint32_t Battery_Voltage;

uint8_t ReadBatVolt(uint32_t* Volt);

#endif // _VOLT_H_