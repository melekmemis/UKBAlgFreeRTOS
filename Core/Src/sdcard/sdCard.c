/*
 * sdCard.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "sdCard.h"
#include "fatfs.h"
#include "main.h"
#include "string.h"

FATFS FatFs;
FIL fil;
FRESULT fres;

int dataSize = 0;

void sdCardOpen() {
	fres = f_mount(&FatFs, "", 1);
	if (fres != FR_OK) { }
}

void sdCardClose() {
	f_mount(NULL, "", 0);
}

void sdCardWrite() {
	fres = f_open(&fil, "yeni.csv", FA_WRITE | FA_OPEN_APPEND);
	f_lseek(&fil, f_size(&fil));
	if(fres == FR_OK) {
	} else {
	}
	char str[200];
//	sprintf(str, "Time: %d:%d:%d, accX: %f, accY: %f, accZ: %f, gyroX: %f, gyroY: %f, gyroZ: %f, Altitude: %f\r\n",
//		    	             sensorData.saat, sensorData.dakika, sensorData.saniye, sensorData.Ax, sensorData.Ay, sensorData.Az, sensorData.Gx, sensorData.Gy, sensorData.Gz, altitude);

	dataSize = (int)strlen(str);

	UINT bytesWrote;
	fres = f_write(&fil, str, dataSize, &bytesWrote); ///////str ring buffera atılacak her döngüde çalışmayacak sd kart
	if(fres == FR_OK) {
//	    yazdi = 1;
	} else {
//	    yazdi = 0;
	}

	f_close(&fil);
}
