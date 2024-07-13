#include "ICM42688.h"
#include "i2c.h"
#include"Utilities.h"

#define ICM42688_ADDR  0xD0
#define REG_BANK_SEL  0x76
#define ICM42688_TIMEOUT 500

/* User Register Bank0 */
#define REG_DEVICE_CONFIG 0x11
#define REG_INT_CONFIG  0x14
#define REG_ACCEL_DATA_X1  0x1f
#define REG_ACCEL_DATA_X0  0x20
#define REG_ACCEL_DATA_Y1  0x21
#define REG_ACCEL_DATA_Y0  0x22
#define REG_ACCEL_DATA_Z1  0x23
#define REG_ACCEL_DATA_Z0  0x24
#define REG_GYRO_DATA_X1  0x25
#define REG_GYRO_DATA_X0  0x26
#define REG_GYRO_DATA_Y1  0x27
#define REG_GYRO_DATA_Y0  0x28
#define REG_GYRO_DATA_Z1  0x29
#define REG_GYRO_DATA_Z0  0x2A
#define REG_INT_STATUS2  0x37
#define REG_INT_STATUS3  0x38
#define REG_INTF_CONFIG1  0x4D
#define REG_PWR_MGMT0  0x4E
#define REG_GYRO_CONFIG0  0x4F
#define REG_ACCEL_CONFIG0  0x50
#define REG_WHO_AM_I  0x75
#define REG_APEX_CONFIG0 0x56
#define REG_SMD_CONFIG 0x57
#define REG_INT_SOURCE1 0x66

/* User Register Bank1 */
#define REG_SENSOR_CONFIG0  0x3

/* User Register Bank4 */
#define REG_ACCEL_WOM_X_THR 0x4A
#define REG_ACCEL_WOM_Y_THR 0x4B
#define REG_ACCEL_WOM_Z_THR 0x4C
#define REG_INT_SOURCE6 0x4D

uint8_t ICM42688_WriteReg(uint8_t reg, uint8_t val)
{
    if (HAL_I2C_Mem_Write(&ICM42688_I2cHandle, ICM42688_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, ICM42688_TIMEOUT) != HAL_OK) return 1;
    return 0;
}

uint8_t ICM42688_ReadReg(uint8_t reg, uint8_t* val)
{
    if (HAL_I2C_Mem_Read(&ICM42688_I2cHandle, ICM42688_ADDR, reg, I2C_MEMADD_SIZE_8BIT, val, 1, ICM42688_TIMEOUT) != HAL_OK) return 1;
    return 0;
}

uint8_t ICM42688_RegBankSelect(uint8_t RegBank)
{
    switch (RegBank)
    {
    case 0:
        if (ICM42688_WriteReg(REG_BANK_SEL, 0x0) != 0) return 1;
        break;
    case 1:
        if (ICM42688_WriteReg(REG_BANK_SEL, 0x1) != 0) return 2;
        break;
    case 2:
        if (ICM42688_WriteReg(REG_BANK_SEL, 0x2) != 0) return 3;
        break;
    case 4:
        if (ICM42688_WriteReg(REG_BANK_SEL, 0x4) != 0) return 3;
        break;
    default:
        return 4;
        break;
    }
    return 0;
}

uint8_t ICM42688_WOMConfig(uint8_t WOM_X_TH, uint8_t WOM_Y_TH, uint8_t WOM_Z_TH)
{
    if (ICM42688_RegBankSelect(4) != 0) return 1;

    if (ICM42688_WriteReg(REG_ACCEL_WOM_X_THR, WOM_X_TH) != 0) return 2;
    if (ICM42688_WriteReg(REG_ACCEL_WOM_Y_THR, WOM_Y_TH) != 0) return 3;
    if (ICM42688_WriteReg(REG_ACCEL_WOM_Z_THR, WOM_Z_TH) != 0) return 4;

    if (ICM42688_RegBankSelect(0) != 0) return 5;
    if (ICM42688_WriteReg(REG_INT_SOURCE1, 0x07) != 0) return 6;
    if (ICM42688_WriteReg(REG_SMD_CONFIG, 0x4) != 0) return 7;

    return 0;
}

uint8_t ICM42688_Init(uint8_t AccelSensitivity[3], uint8_t GyroSensitivity[3])
{
    uint8_t DeviceID, ICM42688_PWR_MGMT0;

    ICM42688_ReadReg(REG_WHO_AM_I, &DeviceID);
    if (ICM42688_WriteReg(REG_DEVICE_CONFIG, 0x01) != 0) return 1;
    HAL_Delay(3);

    if (DeviceID != 0x47) return 2;
    if (ICM42688_ReadReg(REG_PWR_MGMT0, &ICM42688_PWR_MGMT0) != 0) return 3;
    if ((AccelSensitivity[0] | AccelSensitivity[1] | AccelSensitivity[2]) != 0)
    {
        if (ICM42688_WriteReg(REG_ACCEL_CONFIG0, 0x29) != 0) return 4;
        if (ICM42688_WOMConfig(AccelSensitivity[0], AccelSensitivity[1], AccelSensitivity[2]) != 0) return 5;
        ICM42688_PWR_MGMT0 |= (1 << 0);
        ICM42688_PWR_MGMT0 |= (1 << 1);
    }
    if ((GyroSensitivity[0] | GyroSensitivity[1] | GyroSensitivity[2]) != 0)
    {
        ICM42688_PWR_MGMT0 |= (1 << 2);
        ICM42688_PWR_MGMT0 |= (1 << 3);
    }
    if (ICM42688_WriteReg(REG_PWR_MGMT0, ICM42688_PWR_MGMT0) != 0)
    {
        return 6;
    }
    HAL_Delay(1);
    return 0;
}

// /*@note Put Params to 1 to shutdown them */
// uint8_t ICM42688_LowPowerMode(uint8_t Accel, uint8_t Gyro)
// {
//     uint8_t ICM42688_PWR_MGMT0;
//     if (ICM42688_ReadReg(REG_PWR_MGMT0, &ICM42688_PWR_MGMT0) != 0) return 1;
//     if (Accel == 1)
//     {
//         /* Set Accel to Low Power mode (Register 0x4Eh in Bank 0)
// ACCEL_MODE = 2 and (Register 0x4Dh in Bank 0), ACCEL_LP_CLK_SEL = 0, for low power mode */
// /* Set TH to 100 */
//         /* INT_SOURCE4  */
//         /* 0x56h  */
//         ICM42688_PWR_MGMT0 &= ~(1 << 0);
//         ICM42688_PWR_MGMT0 |= (1 << 1);
//     }
//     if (Gyro == 1)
//     {
//         ICM42688_PWR_MGMT0 &= ~(1 << 2);
//         ICM42688_PWR_MGMT0 &= ~(1 << 3);
//     }
//     if (ICM42688_WriteReg(REG_PWR_MGMT0, ICM42688_PWR_MGMT0) != 0)
//     {
//         return 2;
//     }
//     return 0;
// }