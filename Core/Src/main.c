/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os2.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
#include "system_init.h"
#include "ukbTasks.h"
#include "kalmanFilter.h"
#include "bno055UKB.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart3_rx;

extern osThreadId_t BnoTaskHandle;
extern const osThreadAttr_t BnoTask_attributes;
extern osThreadId_t GPSTaskHandle;
extern const osThreadAttr_t GPSTask_attributes;
extern osThreadId_t MsTaskHandle;
extern const osThreadAttr_t MsTask_attributes;
extern osThreadId_t RsTaskHandle;
extern const osThreadAttr_t RsTask_attributes;
extern osThreadId_t RsSendTaskHandle;
extern const osThreadAttr_t RsSendTask_attributes;
extern osThreadId_t LoRaTaskHandle;
extern const osThreadAttr_t LoRaTask_attributes;
extern osThreadId_t flightStatTaskHandle;
extern const osThreadAttr_t flightStatTask_attributes;
extern osThreadId_t sdCardTaskHandle;
extern const osThreadAttr_t sdCardTask_attributes;

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  Kalman_Init_All();
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();

  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 1);

  HAL_Delay(500);
  bno055_initialize();

//  HAL_GPIO_WritePin(drogueOut_GPIO_Port, drogueOut_Pin, 1);
//  HAL_Delay(600);
//  HAL_GPIO_WritePin(drogueOut_GPIO_Port, drogueOut_Pin, 0);
//  HAL_GPIO_WritePin(mainOut_GPIO_Port, mainOut_Pin, 1);
//  HAL_Delay(600);
//  HAL_GPIO_WritePin(mainOut_GPIO_Port, mainOut_Pin, 0);

  HAL_Delay(50);
  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0);

  /* Init scheduler */
  osKernelInitialize();

  createTasks();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  while (1)
  {

  }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
