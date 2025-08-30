/*
 * nmea_parse.h
 *
 *  Created on: Feb 21, 2025
 *      Author: memis
 */

#ifndef NMEA_PARSE_H_
#define NMEA_PARSE_H_


#include <stm32f4xx.h>

#define DataBuffer_SIZE 2000

typedef struct NMEA_DATA {
    double latitude;
    char latSide;
    double longitude;
    char lonSide;
    float altitude;
    float hdop;
    int satelliteCount;
    int fix;
    char lastMeasure[10];
} GPS;

void nmea_parse(GPS *gps_data, uint8_t *buffer);
void ringBuffer(uint8_t *buffer, size_t Size);
#endif /* NMEA_PARSE_H_ */


