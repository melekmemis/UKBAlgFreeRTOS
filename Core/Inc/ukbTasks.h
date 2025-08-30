/*
 * ukbTasks.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef UKBTASKS_H_
#define UKBTASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void createTasks();

void BnoTaskInit(void *argument);
void GPSTaskInit(void *argument);
void MsTaskInit(void *argument);
void RsTaskInit(void *argument);
void RsSendTaskInit(void *argument);
void LoRaTaskInit(void *argument);
void flightStatTaskInit(void *argument);
void sdCardTaskInit(void *argument);

void Restart_UART_DMA_RX();
#ifdef __cplusplus
}
#endif

#endif /* UKBTASKS_H_ */
