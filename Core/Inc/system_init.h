/*
 * system_init.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef SYSTEM_INIT_H_
#define SYSTEM_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_I2C1_Init(void);
void MX_I2C2_Init(void);
void MX_SPI3_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_INIT_H_ */
