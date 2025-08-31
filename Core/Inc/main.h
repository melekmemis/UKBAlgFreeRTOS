/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define drogueOut_Pin GPIO_PIN_2
#define drogueOut_GPIO_Port GPIOE
#define loraRST_Pin GPIO_PIN_3
#define loraRST_GPIO_Port GPIOE
#define loraAux_Pin GPIO_PIN_1
#define loraAux_GPIO_Port GPIOC
#define buzzer_Pin GPIO_PIN_4
#define buzzer_GPIO_Port GPIOC
#define bnoRST_Pin GPIO_PIN_10
#define bnoRST_GPIO_Port GPIOE
#define bnoINT_Pin GPIO_PIN_11
#define bnoINT_GPIO_Port GPIOE
#define bnoPS0_Pin GPIO_PIN_12
#define bnoPS0_GPIO_Port GPIOE
#define bnoPS1_Pin GPIO_PIN_13
#define bnoPS1_GPIO_Port GPIOE
#define bnoADR_Pin GPIO_PIN_15
#define bnoADR_GPIO_Port GPIOE
#define gpsRST_Pin GPIO_PIN_13
#define gpsRST_GPIO_Port GPIOB
#define mainOut_Pin GPIO_PIN_11
#define mainOut_GPIO_Port GPIOA
#define sdSS_Pin GPIO_PIN_15
#define sdSS_GPIO_Port GPIOA
#define loraM0_Pin GPIO_PIN_8
#define loraM0_GPIO_Port GPIOB
#define loraM1_Pin GPIO_PIN_1
#define loraM1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern RTC_HandleTypeDef hrtc;

extern SPI_HandleTypeDef hspi3;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;

#define SD_SPI_HANDLE hspi3
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
