#ifndef LORALIB_H
#define LORALIB_H

#include <stm32f4xx.h>
#include "stdint.h"
#include "stdbool.h"
#include <string.h>

#pragma pack(1)
typedef struct {
//	uint8_t aa;
	float enlem;
	float boylam;
	float gpsIrtifa;
    float irtifa;
    int uyduSayisi;
    int saat;
    int dakika;
    int saniye;
    float Ax;
    float Ay;
    float Az;
    float Gx;
    float Gy;
    float Gz;
    float angleX;
    float angleY;
    float angleZ;
    uint8_t durum;
} SensorData;
#pragma pack()

extern SensorData sensorData;

void LoraConfigure(void);
void LoraSendData(UART_HandleTypeDef *huart, uint8_t addrH, uint8_t addrL, uint8_t channel, void *data, size_t size);

#endif
