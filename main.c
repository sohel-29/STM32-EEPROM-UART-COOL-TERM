/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define EEPROM_ADDR        (0x50 << 1)
#define EEPROM_PAGE_SIZE   64
#define UART_RX_BUF_SIZE   100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
uint8_t eeprom_rx_buf[UART_RX_BUF_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UART_Print(char *msg);
uint16_t UART_Read_Line(uint8_t *buf, uint16_t max_len);
HAL_StatusTypeDef EEPROM_Write(uint16_t mem_addr, uint8_t *data, uint16_t size);
HAL_StatusTypeDef EEPROM_Read(uint16_t mem_addr, uint8_t *data, uint16_t size);
HAL_StatusTypeDef EEPROM_Write_Buffer(uint16_t addr, uint8_t *data, uint16_t size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void UART_Print(char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

uint16_t UART_Read_Line(uint8_t *buf, uint16_t max_len)
{
  uint8_t ch;
  uint16_t idx = 0;

  while (idx < (max_len - 1))
  {
    if (HAL_UART_Receive(&huart2, &ch, 1, HAL_MAX_DELAY) == HAL_OK)
    {
      HAL_UART_Transmit(&huart2, &ch, 1, HAL_MAX_DELAY);

      if (ch == '\r' || ch == '\n')
      {
        break;
      }

      buf[idx++] = ch;
    }
  }

  buf[idx] = '\0';
  return idx;
}

HAL_StatusTypeDef EEPROM_Write(uint16_t mem_addr, uint8_t *data, uint16_t size)
{
  HAL_StatusTypeDef status;

  status = HAL_I2C_Mem_Write(&hi2c1,
                             EEPROM_ADDR,
                             mem_addr,
                             I2C_MEMADD_SIZE_16BIT,
                             data,
                             size,
                             HAL_MAX_DELAY);

  if (status != HAL_OK)
  {
    return status;
  }

  while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 10, 100) != HAL_OK);

  return HAL_OK;
}

HAL_StatusTypeDef EEPROM_Read(uint16_t mem_addr, uint8_t *data, uint16_t size)
{
  return HAL_I2C_Mem_Read(&hi2c1,
                          EEPROM_ADDR,
                          mem_addr,
                          I2C_MEMADD_SIZE_16BIT,
                          data,
                          size,
                          HAL_MAX_DELAY);
}

HAL_StatusTypeDef EEPROM_Write_Buffer(uint16_t addr, uint8_t *data, uint16_t size)
{
  uint16_t bytes_written = 0;

  while (bytes_written < size)
  {
    uint16_t page_offset = addr % EEPROM_PAGE_SIZE;
    uint16_t space = EEPROM_PAGE_SIZE - page_offset;
    uint16_t chunk = ((size - bytes_written) < space) ? (size - bytes_written) : space;

    if (EEPROM_Write(addr, &data[bytes_written], chunk) != HAL_OK)
    {
      return HAL_ERROR;
    }

    addr += chunk;
    bytes_written += chunk;
  }

  return HAL_OK;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint16_t len;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  UART_Print("SYSTEM START\r\n");

  if (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 5, 100) != HAL_OK)
  {
    UART_Print("EEPROM NOT DETECTED\r\n");
    while (1);
  }

  UART_Print("EEPROM READY\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* USER CODE BEGIN WHILE */
    UART_Print("\r\nEnter text: ");

    memset(uart_rx_buf, 0, sizeof(uart_rx_buf));
    memset(eeprom_rx_buf, 0, sizeof(eeprom_rx_buf));

    len = UART_Read_Line(uart_rx_buf, UART_RX_BUF_SIZE);

    if (len == 0)
    {
      UART_Print("\r\nNo data entered\r\n");
      continue;
    }

    if (EEPROM_Write_Buffer(0x0000, uart_rx_buf, len) == HAL_OK)
    {
      UART_Print("\r\nWRITE OK\r\n");
    }
    else
    {
      UART_Print("\r\nWRITE FAIL\r\n");
      continue;
    }

    HAL_Delay(10);

    if (EEPROM_Read(0x0000, eeprom_rx_buf, len) == HAL_OK)
    {
      UART_Print("READ OK\r\n");
    }
    else
    {
      UART_Print("READ FAIL\r\n");
      continue;
    }

    UART_Print("UART DATA   : ");
    UART_Print((char *)uart_rx_buf);
    UART_Print("\r\n");

    UART_Print("EEPROM DATA : ");
    UART_Print((char *)eeprom_rx_buf);
    UART_Print("\r\n--------------------------\r\n");
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
