/*
    ICM42688.h
    Created on Jun 30, 2024
    Author: @POEG1726
*/

#ifndef _ICM42688_H_
#define _ICM42688_H_

#include "stm32g0xx.h"

#define ICM42688_I2cHandle (hi2c2)

uint8_t ICM42688_Init(uint8_t AccelSensitivity[3], uint8_t GyroSensitivity[3]);
// uint8_t ICM42688_LowPowerMode(uint8_t Accel, uint8_t Gyro);
#endif // _ICM42688_H_