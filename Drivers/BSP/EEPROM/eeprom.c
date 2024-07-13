#include "eeprom.h"
#include "i2c.h"

#define EEPROM_ADDR 0xA0
// static const uint8_t EEPROM_ADDR = 0xB0;
#define EEPROM_MEM_SIZE 0x1fff
#define EEPROM_PAGE_SIZE 0x20
#define EEPROM_I2C_TIMEOUT 1000

uint8_t EEPROM_Check()
{
    uint8_t read_data = 0x00;
    uint16_t test_address = 0x0000;

    EEPROM_ReadByte(test_address, &read_data);

    if (read_data != 0xAA)
    {
        EEPROM_WriteByte(test_address, 0xAA);
        HAL_Delay(5);

        EEPROM_ReadByte(test_address,&read_data);
        if (read_data != 0xAA) return 1;
    }

    return 0;
}

void EEPROM_WriteByte(uint16_t addr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&EEPROM_I2C_HANDLER, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &byte, 1, EEPROM_I2C_TIMEOUT);
    // HAL_Delay(5);
}

void EEPROM_WritePage(uint16_t addr, uint8_t* buffer)
{
    HAL_I2C_Mem_Write(&EEPROM_I2C_HANDLER, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, buffer, EEPROM_PAGE_SIZE, EEPROM_I2C_TIMEOUT);
}

void EEPROM_WriteBuffer(uint16_t addr, uint8_t* buffer, uint16_t buffer_size)
{
    uint16_t remaining = buffer_size;
    uint16_t write_addr = addr;
    uint8_t* write_buffer = buffer;

    while (remaining > 0)
    {
        uint16_t chunk_size = remaining < EEPROM_PAGE_SIZE ? remaining : EEPROM_PAGE_SIZE;
        EEPROM_WritePage(write_addr, write_buffer);
        write_addr += chunk_size;
        write_buffer += chunk_size;
        remaining -= chunk_size;
        HAL_Delay(5);
    }
}

void EEPROM_ReadByte(uint16_t addr, uint8_t* byte)
{
    HAL_I2C_Mem_Read(&EEPROM_I2C_HANDLER, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, byte, 1, EEPROM_I2C_TIMEOUT);
}

void EEPROM_ReadBuffer(uint16_t addr, uint8_t* buffer, uint16_t buffer_size)
{
    HAL_I2C_Mem_Read(&EEPROM_I2C_HANDLER, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, buffer, buffer_size, EEPROM_I2C_TIMEOUT);
}