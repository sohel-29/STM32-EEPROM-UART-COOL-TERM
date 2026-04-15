/*
 * z_i2c_eeprom.c
 *
 * STM32 HAL I2C EEPROM Driver
 * Compatible with 24C256
 */

#include <string.h>
#include "main.h"
#include "z_i2c_eeprom.h"

/* Use I2C handle from main.c */
extern I2C_HandleTypeDef hi2c1;

/*
 * Optional test function
 * Not needed for production
 */
void test_EEPROM(void)
{
    uint8_t result[10] = {0};
    uint16_t delta[10] = {0};
    char dataIn[EE_PAGE_LEN] = {0};
    char dataOut[EE_PAGE_LEN] = {0};
    uint16_t time;
    uint8_t k = 0;

    k = 0;
    TIM7->EGR = 1;

    HAL_Delay(20);
    delta[k++] = TIM7->CNT;

    time = TIM7->CNT;
    result[k] = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR,
                                 I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataIn, 1, I2C_COMM_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR,
                                 I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataIn, EE_PAGE_LEN, I2C_COMM_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR,
                                  I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataOut, 1, I2C_COMM_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR,
                                  I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataOut, EE_PAGE_LEN, I2C_COMM_TIMEOUT);
    delta[k++] = TIM7->CNT - time;

    time = TIM7->CNT;
    result[k] = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    delta[k++] = TIM7->CNT - time;
}

/***************************************
 * @brief  Read one full EEPROM page
 * @param  addr  Memory address
 * @param  data  Buffer to store page data
 * @retval HAL status
 ***************************************/
HAL_StatusTypeDef read_p_EEPROM(uint16_t addr, char *data)
{
    HAL_StatusTypeDef result;

    result = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    if (result == HAL_OK)
    {
        result = HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, addr,
                                  I2C_MEMADD_SIZE_16BIT, (uint8_t *)data,
                                  EE_PAGE_LEN, I2C_COMM_TIMEOUT);
    }
    return result;
}

/***************************************
 * @brief  Read n bytes from EEPROM
 * @param  addr  Memory address
 * @param  data  Output buffer
 * @param  size  Number of bytes to read
 * @retval HAL status
 ***************************************/
HAL_StatusTypeDef read_n_EEPROM(uint16_t addr, char *data, uint8_t size)
{
    HAL_StatusTypeDef result;

    result = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    if (result == HAL_OK)
    {
        result = HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, addr,
                                  I2C_MEMADD_SIZE_16BIT, (uint8_t *)data,
                                  size, I2C_COMM_TIMEOUT);
    }
    return result;
}

/***************************************
 * @brief  Write null-terminated string
 * @param  addr  Memory address
 * @param  data  String to write
 * @retval HAL status
 ***************************************/
HAL_StatusTypeDef write_s_EEPROM(uint16_t addr, char *data)
{
    HAL_StatusTypeDef result;

    result = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    if (result == HAL_OK)
    {
        result = HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, addr,
                                   I2C_MEMADD_SIZE_16BIT, (uint8_t *)data,
                                   strlen(data) + 1, I2C_COMM_TIMEOUT);
    }
    return result;
}

/***************************************
 * @brief  Write n bytes to EEPROM
 * @param  addr  Memory address
 * @param  data  Data buffer
 * @param  size  Number of bytes to write
 * @retval HAL status
 ***************************************/
HAL_StatusTypeDef write_n_EEPROM(uint16_t addr, char *data, uint8_t size)
{
    HAL_StatusTypeDef result;

    result = HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
    if (result == HAL_OK)
    {
        result = HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, addr,
                                   I2C_MEMADD_SIZE_16BIT, (uint8_t *)data,
                                   size, I2C_COMM_TIMEOUT);
    }
    return result;
}
