/*
 * nmea_parse.h
 *
 *  Created on: Feb 21, 2025
 *      Author: memis
 */

#include "nmea_parse.h"
#include <string.h>
#include <stdlib.h>

#define RxBuffer_SIZE 1000

uint16_t oldPos = 0;
uint16_t newPos = 0;
uint8_t DataBuffer[DataBuffer_SIZE];
extern uint8_t RxBuffer[RxBuffer_SIZE];
char *data[15];
int cnt = 0;

int gps_checksum(char *nmea_data)
{
    if(strlen(nmea_data) < 5) return 0;
    char recv_crc[2];
    recv_crc[0] = nmea_data[strlen(nmea_data) - 4];
    recv_crc[1] = nmea_data[strlen(nmea_data) - 3];
    int crc = 0;
    int i;

    for (i = 0; i < strlen(nmea_data) - 5; i ++) {
        crc ^= nmea_data[i];
    }
    int receivedHash = strtol(recv_crc, NULL, 16);
    if (crc == receivedHash) {
        return 1;
    }
    else{
        return 0;
    }
}

int nmea_GPGGA(GPS *gps_data, char*inputString){

	char values[25][20];
	    int counter = 0;

	    char *marker = strtok(inputString, ",");
	    while (marker != NULL && counter < 25) {
	        strncpy(values[counter], marker, sizeof(values[counter]) - 1);
	        values[counter][sizeof(values[counter]) - 1] = '\0';
	        marker = strtok(NULL, ",");
	        counter++;
	    }

	    char lonSide = values[5][0];
	    char latSide = values[3][0];
	    strcpy(gps_data->lastMeasure, values[1]);

	    if(latSide == 'S' || latSide == 'N'){
	        char lat_d[3] = {0};
	        char lat_m[8] = {0};
	        strncpy(lat_d, values[2], 2);
	        strncpy(lat_m, values[2] + 2, 6);

	        int lat_deg_strtol = strtol(lat_d, NULL, 10);
	        float lat_min_strtof = strtof(lat_m, NULL);
	        double lat_deg = lat_deg_strtol + lat_min_strtof / 60.0;

	        char lon_d[4] = {0};
	        char lon_m[8] = {0};
	        strncpy(lon_d, values[4], 3);
	        strncpy(lon_m, values[4] + 3, 6);

	        int lon_deg_strtol = strtol(lon_d, NULL, 10);
	        float lon_min_strtof = strtof(lon_m, NULL);
	        double lon_deg = lon_deg_strtol + lon_min_strtof / 60.0;

	        if(lat_deg != 0 && lon_deg != 0 && lat_deg < 90 && lon_deg < 180){
	            gps_data->latitude = lat_deg;
	            gps_data->latSide = latSide;
	            gps_data->longitude = lon_deg;
	            gps_data->lonSide = lonSide;

	            float altitude = strtof(values[9], NULL);
	            gps_data->altitude = altitude != 0 ? altitude : gps_data->altitude;
	            gps_data->satelliteCount = strtol(values[7], NULL, 10);

	            int fixQuality = strtol(values[6], NULL, 10);
	            gps_data->fix = fixQuality > 0 ? 1 : 0;

	            float hdop = strtof(values[8], NULL);
	            gps_data->hdop = hdop != 0 ? hdop : gps_data->hdop;

	            return 1;
	        } else {
	            return 0;
	        }
	    }

	    return 0;
}


int nmea_GPGSA(GPS *gps_data, char*inputString){

#define MAX_VALUES 25
#define MAX_TOKEN_LENGTH 20

char values[MAX_VALUES][MAX_TOKEN_LENGTH];
int counter = 0;

char *marker = strtok(inputString, ",");
while (marker != NULL && counter < MAX_VALUES) {
    strncpy(values[counter], marker, MAX_TOKEN_LENGTH - 1);
    values[counter][MAX_TOKEN_LENGTH - 1] = '\0';
    counter++;
    marker = strtok(NULL, ",");
}

int fix = strtol(values[2], NULL, 10);
gps_data->fix = fix > 1 ? 1 : 0;

int satelliteCount = 0;
for (int i = 3; i <= 14 && i < counter; i++) {
    if (values[i][0] != '\0') {
        satelliteCount++;
    }
}
gps_data->satelliteCount = satelliteCount;

return 1;

}

int nmea_GPGLL(GPS *gps_data, char*inputString) {

	char values[25][20];
	    int counter = 0;

	    char *marker = strtok(inputString, ",");
	    while (marker != NULL && counter < 25) {
	        strncpy(values[counter], marker, sizeof(values[counter]) - 1);
	        values[counter][sizeof(values[counter]) - 1] = '\0';
	        marker = strtok(NULL, ",");
	        counter++;
	    }

	    char latSide = values[2][0];
	    if (latSide == 'S' || latSide == 'N') {
	        char lat_d[3] = {0};
	        char lat_m[8] = {0};
	        strncpy(lat_d, values[1], 2);
	        strncpy(lat_m, values[1] + 2, 6);

	        int lat_deg_strtol = strtol(lat_d, NULL, 10);
	        float lat_min_strtof = strtof(lat_m, NULL);
	        double lat_deg = lat_deg_strtol + lat_min_strtof / 60.0;

	        char lon_d[4] = {0};
	        char lon_m[8] = {0};
	        strncpy(lon_d, values[3], 3);
	        strncpy(lon_m, values[3] + 3, 6);
	        char lonSide = values[4][0];

	        int lon_deg_strtol = strtol(lon_d, NULL, 10);
	        float lon_min_strtof = strtof(lon_m, NULL);
	        double lon_deg = lon_deg_strtol + lon_min_strtof / 60.0;

	        if (lon_deg_strtol == 0 || lon_min_strtof == 0 || lat_deg_strtol == 0 || lat_min_strtof == 0) {
	            return 0;
	        } else {
	            gps_data->latitude = lat_deg;
	            gps_data->longitude = lon_deg;
	            gps_data->latSide = latSide;
	            gps_data->lonSide = lonSide;
	            return 1;
	        }
	    }

	    return 0;
}

void nmea_parse(GPS *gps_data, uint8_t *buffer){
    memset(data, 0, sizeof(data));
    char * token = strtok(buffer, "$");
    cnt = 0;

    while (token != NULL) {
            if (cnt < 15) {
                data[cnt++] = token;
            }
            token = strtok(NULL, "$");
        }

    for(int i = 0; i<cnt; i++){
       if(strstr(data[i], "\r\n")!=NULL && gps_checksum(data[i])){
           if(strstr(data[i], "GPGLL")!=NULL || strstr(data[i], "GNGLL")!=NULL){
               nmea_GPGLL(gps_data, data[i]);
           }
           else if(strstr(data[i], "GPGSA")!=NULL || strstr(data[i], "GNGSA")!=NULL){
               nmea_GPGSA(gps_data, data[i]);
           }
//           if(strstr(data[i], "GPGSA")!=NULL){
//                 nmea_GPGSA(gps_data, data[i]);
//           }
           else if(strstr(data[i], "GPGGA")!=NULL || strstr(data[i], "GNGGA")!=NULL){
               nmea_GPGGA(gps_data, data[i]);
           }
       }

    }


}

void ringBuffer(uint8_t *buffer, size_t Size) {  //rxBuffer tasks.c de buradan nasıl görüyor?
    oldPos = newPos;
    if(oldPos + Size > DataBuffer_SIZE){

        uint16_t datatocopy = DataBuffer_SIZE-oldPos;
        memcpy ((uint8_t *)DataBuffer+oldPos, RxBuffer, datatocopy);

        oldPos = 0;
        memcpy ((uint8_t *)DataBuffer, (uint8_t *)RxBuffer+datatocopy, (Size-datatocopy));
        newPos = (Size-datatocopy);
    }
    else{
        memcpy((uint8_t *)DataBuffer+oldPos, RxBuffer, Size);
        newPos = Size+oldPos;

    }
}
