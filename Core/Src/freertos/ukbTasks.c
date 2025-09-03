/*
 * tasks.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "cmsis_os2.h"
#include "freertos.h"
#include "task.h"
#include "ukbTasks.h"
#include "string.h"
#include "bno055UKB.h"
#include "ms5611UKB.h"
#include "testDevice.h"
#include "nmea_parse.h"
#include "loraLib.h"
#include "flightStatus.h"
#include "sdCard.h"

/* Definitions for BnoTask */
osThreadId_t BnoTaskHandle;
const osThreadAttr_t BnoTask_attributes = {
  .name = "BnoTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GPSTask */
osThreadId_t GPSTaskHandle;
const osThreadAttr_t GPSTask_attributes = {
  .name = "GPSTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MsTask */
osThreadId_t MsTaskHandle;
const osThreadAttr_t MsTask_attributes = {
  .name = "MsTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RsTask */
osThreadId_t RsTaskHandle;
const osThreadAttr_t RsTask_attributes = {
  .name = "RsTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for RsSendTask */
osThreadId_t RsSendTaskHandle;
const osThreadAttr_t RsSendTask_attributes = {
  .name = "RsSendTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for LoRaTask */
osThreadId_t LoRaTaskHandle;
const osThreadAttr_t LoRaTask_attributes = {
  .name = "LoRaTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for flightStatTask */
osThreadId_t flightStatTaskHandle;
const osThreadAttr_t flightStatTask_attributes = {
  .name = "flightStatTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sdCardTask */
osThreadId_t sdCardTaskHandle;
const osThreadAttr_t sdCardTask_attributes = {
  .name = "sdCardTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void createTasks() {
	/* Create the thread(s) */
	/* creation of BnoTask */
	BnoTaskHandle = osThreadNew(BnoTaskInit, NULL, &BnoTask_attributes);

	/* creation of GPSTask */
	GPSTaskHandle = osThreadNew(GPSTaskInit, NULL, &GPSTask_attributes);

	/* creation of MsTask */
	MsTaskHandle = osThreadNew(MsTaskInit, NULL, &MsTask_attributes);

	/* creation of RsTask */
	RsTaskHandle = osThreadNew(RsTaskInit, NULL, &RsTask_attributes);

	/* creation of RsSendTask */
	RsSendTaskHandle = osThreadNew(RsSendTaskInit, NULL, &RsSendTask_attributes);

	/* creation of LoRaTask */
	LoRaTaskHandle = osThreadNew(LoRaTaskInit, NULL, &LoRaTask_attributes);

	/* creation of flightStatTask */
	flightStatTaskHandle = osThreadNew(flightStatTaskInit, NULL, &flightStatTask_attributes);

	/* creation of sdCardTask */
	sdCardTaskHandle = osThreadNew(sdCardTaskInit, NULL, &sdCardTask_attributes);
}

/* USER CODE BEGIN Header_BnoTaskInit */
/**
  * @brief  Function implementing the BnoTask thread.
  * @param  argument: Not used
  * @retval None
  */
uint8_t bnoSayac = 0;
/* USER CODE END Header_BnoTaskInit */
void BnoTaskInit(void *argument)
{
  /* USER CODE BEGIN 5 */
	bno055_initialize();
  /* Infinite loop */
  for(;;)
  {
      if(mode == sitMode || mode == normalMode) {
    	  bnoSayac++;
    	  if (bnoSayac == 60) {
    		  bnoSayac = 0;
    	  }
//	  	  if (bnoSayac % 2 == 0) {
//	  		  HAL_GPIO_TogglePin(blueLed_GPIO_Port, blueLed_Pin);
//	  	  }
    	  bno055ReadAngles();
      }
	  osDelay(100);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_GPSTaskInit */
/**
* @brief Function implementing the GPSTask thread.
* @param argument: Not used
* @retval None
*/

GPS myData;
#define RxBuffer_SIZE 1000
uint8_t RxBuffer[RxBuffer_SIZE];
uint32_t sayac = 0;
uint32_t sizeGps = 0;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
    	sizeGps = Size;

//		if (sayac == 60) {
//			sayac = 0;
//		}
		sayac++;
//        ringBuffer(RxBuffer, Size);

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(GPSTaskHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    if (huart->Instance == USART3)
    {
    	recievedSize = Size;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(RsTaskHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/* USER CODE END Header_GPSTaskInit */
void GPSTaskInit(void *argument)
{
  /* USER CODE BEGIN GPSTaskInit */
	  HAL_StatusTypeDef uart_status = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, RxBuffer_SIZE);
	  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	  if (uart_status != HAL_OK) {
		  Restart_UART_DMA_RX();
	  }
  /* Infinite loop */
  for(;;)
  {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//      HAL_GPIO_TogglePin(redLed_GPIO_Port, redLed_Pin);
      nmea_parse(&myData, RxBuffer);

//      if (mode == normalMode){
          HAL_StatusTypeDef uart_status = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, RxBuffer_SIZE);
          __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
          if (uart_status != HAL_OK) {
        	  Restart_UART_DMA_RX();
          }
//      }
      osDelay(20);
  }
  /* USER CODE END GPSTaskInit */
}

/* USER CODE BEGIN Header_MsTaskInit */
/**
* @brief Function implementing the MsTask thread.
* @param argument: Not used
* @retval None
*/
uint32_t msSayac = 0;
/* USER CODE END Header_MsTaskInit */
void MsTaskInit(void *argument)
{
  /* USER CODE BEGIN MsTaskInit */
	MS5611_Start();
  /* Infinite loop */
  for(;;)
  {
	  if(mode == sitMode || mode == normalMode) {
		  msSayac++;
		  altitudeControl();
	  }
	  osDelay(90);
  }
  /* USER CODE END MsTaskInit */
}

/* USER CODE BEGIN Header_RsTaskInit */
/**
* @brief Function implementing the RsTask thread.
* @param argument: Not used
* @retval None
*/
uint32_t rsSayac = 0;
/* USER CODE END Header_RsTaskInit */
void RsTaskInit(void *argument)
{
  /* USER CODE BEGIN RsTaskInit */
	firstTimeSut = 0;

	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxDataReceived, rxDataSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
  /* Infinite loop */
  for(;;)
  {
	  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	  rsSayac++;
	  if (recievedSize == commandSize){
		  memcpy(command, rxDataReceived, commandSize);
		  cmd = parseCommand(command);
	  }

	  modeCheck();

	  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxDataReceived, rxDataSize);
	  __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
	  osDelay(10);
  }
  /* USER CODE END RsTaskInit */
}

/* USER CODE BEGIN Header_RsSendTaskInit */
/**
* @brief Function implementing the RsSendTask thread.
* @param argument: Not used
* @retval None
*/
uint32_t rsSendSayac = 0;
/* USER CODE END Header_RsSendTaskInit */
void RsSendTaskInit(void *argument)
{
  /* USER CODE BEGIN RsSendTaskInit */
	statusPackData1 = 0x00;
	statusPacket[0] = 0xAA;
	statusPacket[2] = 0x00;
	statusPacket[3] = 0x1A;
	statusPacket[4] = 0x0D;
	statusPacket[5] = 0x0A;

	testCihazBuffer[0] = 0xAB;
	testCihazBuffer[34] = 0x0D;
	testCihazBuffer[35] = 0x0A;
  /* Infinite loop */
  for(;;)
  {
	  	if (mode == sitMode) {
	  	rsSendSayac++;
		float_to_bytes(altitude, bytes);
		testCihazBuffer[1] = bytes[0];
		testCihazBuffer[2] = bytes[1];
		testCihazBuffer[3] = bytes[2];
		testCihazBuffer[4] = bytes[3];

		float_to_bytes(pressure, bytes);
		testCihazBuffer[5] = bytes[0];
		testCihazBuffer[6] = bytes[1];
		testCihazBuffer[7] = bytes[2];
		testCihazBuffer[8] = bytes[3];

		float_to_bytes((accX*9.81), bytes);
		testCihazBuffer[9] = bytes[0];
		testCihazBuffer[10] = bytes[1];
		testCihazBuffer[11] = bytes[2];
		testCihazBuffer[12] = bytes[3];

		float_to_bytes((accY*9.81), bytes);
		testCihazBuffer[13] = bytes[0];
		testCihazBuffer[14] = bytes[1];
		testCihazBuffer[15] = bytes[2];
		testCihazBuffer[16] = bytes[3];

		float_to_bytes((accZ*9.81), bytes);
		testCihazBuffer[17] = bytes[0];
		testCihazBuffer[18] = bytes[1];
		testCihazBuffer[19] = bytes[2];
		testCihazBuffer[20] = bytes[3];

		float_to_bytes(pitch, bytes);
		testCihazBuffer[21] = bytes[0];
		testCihazBuffer[22] = bytes[1];
		testCihazBuffer[23] = bytes[2];
		testCihazBuffer[24] = bytes[3];

		float_to_bytes(roll, bytes);
		testCihazBuffer[25] = bytes[0];
		testCihazBuffer[26] = bytes[1];
		testCihazBuffer[27] = bytes[2];
		testCihazBuffer[28] = bytes[3];

		float_to_bytes(yaw, bytes);
		testCihazBuffer[29] = bytes[0];
		testCihazBuffer[30] = bytes[1];
		testCihazBuffer[31] = bytes[2];
		testCihazBuffer[32] = bytes[3];

		testCihazBuffer[33] = checkSumCalc2();
		HAL_UART_Transmit(&huart3, testCihazBuffer, 36, HAL_MAX_DELAY);
		}

		if (mode == sutMode) {
			if (firstTimeSut) {
				osDelay(100);
			}
			statusPacket[1] = statusPackData1;
			HAL_UART_Transmit(&huart3, statusPacket, 6, HAL_MAX_DELAY);
		}
		osDelay(100);
  }
  /* USER CODE END RsSendTaskInit */
}

/* USER CODE BEGIN Header_LoRaTaskInit */
/**
* @brief Function implementing the LoRaTask thread.
* @param argument: Not used
* @retval None
*/
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

uint32_t loraSayac = 0;
/* USER CODE END Header_LoRaTaskInit */
void LoRaTaskInit(void *argument)
{
  /* USER CODE BEGIN LoRaTaskInit */
	LoraConfigure();
  /* Infinite loop */
  for(;;)
  {
	  	loraSayac++;


		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//
		sensorData.saat = sTime.Hours;
		sensorData.dakika = sTime.Minutes;
		sensorData.saniye = sTime.Seconds;
	    sensorData.Ax = accX;
	    sensorData.Ay = accY;
	    sensorData.Az = accZ;
	    sensorData.Gx = gyroX;
	    sensorData.Gy = gyroY;
	    sensorData.Gz = gyroZ;
	    sensorData.angleX = roll;
	    sensorData.angleY = pitch;
	    sensorData.angleZ = yaw;
		sensorData.enlem = myData.latitude;
		sensorData.boylam = myData.longitude;
		sensorData.gpsIrtifa = myData.altitude;
		sensorData.uyduSayisi = myData.satelliteCount;
		sensorData.irtifa = altitude;
		sensorData.durum = durum;
		LoraSendData(&huart2, 0x00, 3, 23, &sensorData, sizeof(SensorData));
		osDelay(200);
  }
  /* USER CODE END LoRaTaskInit */
}

/* USER CODE BEGIN Header_flightStatTaskInit */
/**
* @brief Function implementing the flightStatTask thread.
* @param argument: Not used
* @retval None
*/
uint32_t fsSayac = 0;
/* USER CODE END Header_flightStatTaskInit */
void flightStatTaskInit(void *argument)
{
  /* USER CODE BEGIN flightStatTaskInit */
  durum = 1;
  /* Infinite loop */
  for(;;)
  {
	  fsSayac++;
	  checkFlightStatus();
	  osDelay(100);
  }
  /* USER CODE END flightStatTaskInit */
}

/* USER CODE BEGIN Header_sdCardTaskInit */
/**
* @brief Function implementing the sdCardTask thread.
* @param argument: Not used
* @retval None
*/
uint8_t sdCardIsOpen = 1;
/* USER CODE END Header_sdCardTaskInit */
void sdCardTaskInit(void *argument)
{
  /* USER CODE BEGIN sdCardTaskInit */
	dataSize = 0;
	yazdi = 0;
	osDelay(1000);
	sdCardOpen();
  /* Infinite loop */
  for(;;)
  {
	  if(sdCardIsOpen) {
		  sdCardWrite();
	  }
	  if(mainStep == 1 && altitude < 100) {
		  sdCardClose();
		  sdCardIsOpen = 0;
	  }
//	  sdCardOpen();
//	  sdCardWrite();
//	  sdCardClose();
	  osDelay(100);
  }
  /* USER CODE END sdCardTaskInit */
}

uint32_t oldSayac = 0;

void Restart_UART_DMA_RX() {
	oldSayac++;

    __HAL_DMA_DISABLE(&hdma_usart1_rx);
    while((hdma_usart1_rx.Instance->CR) & DMA_SxCR_EN);

    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx, DMA_FLAG_TCIF1_5 | DMA_FLAG_HTIF1_5 |
                         DMA_FLAG_TEIF1_5 | DMA_FLAG_DMEIF1_5 | DMA_FLAG_FEIF1_5);

    CLEAR_BIT(huart1.Instance->CR3, USART_CR3_DMAR);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, RxBuffer_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}
