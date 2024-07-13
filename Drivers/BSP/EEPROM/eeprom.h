/*
    eeprom.h
    Created on Jul 02, 2024
    Author: @POEG1726
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32g0xx.h"

#define EEPROM_I2C_HANDLER (hi2c2)

uint8_t EEPROM_Check(void);
void EEPROM_WriteByte(uint16_t addr, uint8_t byte);
void EEPROM_ReadByte(uint16_t addr, uint8_t* byte);
void EEPROM_WritePage(uint16_t addr, uint8_t* buffer);
void EEPROM_WriteBuffer(uint16_t addr, uint8_t* buffer, uint16_t buffer_size);
void EEPROM_ReadBuffer(uint16_t addr, uint8_t* buffer, uint16_t buffer_size);

#endif // _EEPROM_H_