/*
 * z_i2c_eeprom.h
 *
 * Complete STM32 HAL I2C EEPROM Driver Header
 * Compatible with 24C256 (32KB, 64-byte pages)
 * Created: April 2026 for STM32CubeIDE
 */

#ifndef INC_Z_I2C_EEPROM_H_
#define INC_Z_I2C_EEPROM_H_

/* Includes */
#include "stm32f4xx_hal.h"  // CORRECT for STM32F446xx // Adjust for your MCU family

/* STM32 I2C Handle - Link to main.c */
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_PORT     hi2c1

/* 24C256 EEPROM Configuration */
#define EEPROM_I2C_ADDR     0xA0    // 7-bit: 0x50 << 1 (A0=A1=A2=GND)
#define EE_PAGE_LEN         64      // 64-byte pages
#define UPPER_LIMIT_EEADDR  0x7FFF  // 32KB total capacity

/* HAL Timing Parameters */
#define EE_AVAIL_TRIALS     20
#define EE_AVAIL_TIMEOUT    HAL_MAX_DELAY
#define I2C_COMM_TIMEOUT    50      // ms for write operations

/* Test/Parameter Address */
#define PARAM_EEADDR        0x0000

/* ============================================================================
   API FUNCTIONS - Return HAL_StatusTypeDef for main.c compatibility
   ============================================================================

   read_p_EEPROM()  - Read 1 byte
   read_n_EEPROM()  - Read multiple bytes
   write_s_EEPROM() - Write null-terminated string
   write_n_EEPROM() - Write multiple bytes (single page/chunk)
   test_EEPROM()    - Self-test routine
   ============================================================================ */

HAL_StatusTypeDef read_p_EEPROM(uint16_t addr, char *data);
HAL_StatusTypeDef read_n_EEPROM(uint16_t addr, char *data, uint8_t size);
HAL_StatusTypeDef write_s_EEPROM(uint16_t addr, char *data);
HAL_StatusTypeDef write_n_EEPROM(uint16_t addr, char *data, uint8_t size);

void test_EEPROM(void);

#endif /* INC_Z_I2C_EEPROM_H_ */
