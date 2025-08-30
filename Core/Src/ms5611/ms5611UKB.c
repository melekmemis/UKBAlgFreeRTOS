/*
 * ms5611UKB.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "ms5611UKB.h"
#include "ms5611.h"
#include "math.h"
#include "kalmanFilter.h"

int8_t c;

float yukseklik;
double t, p, seaLevelPressure = 101325.0;
double referencePressure = 1013.25f;
volatile double currentPressure, altitude;
double baselineAltitude;
volatile double temperature;
volatile double pressure;
volatile uint8_t addr;
uint8_t isStart;
int32_t baroAlt;
uint8_t resolution;
uint8_t resolutionPress;
double absoluteAltitude;
MS5611_t ms5611_struct;

void calculateAltitude() {
    altitude = 44330.0 * (1.0 - pow((currentPressure / referencePressure), 0.1903));
    altitude = Kalman_Update(&altitudeFilter, altitude);
}

void MS5611_Start(){

	  MS5611_init(&hi2c1, &ms5611_struct);
	  resolution = MS5611_CMD_CONVERT_D2_1024;
	  resolutionPress = MS5611_CMD_CONVERT_D1_1024;

	  MS5611_read_temp(&hi2c1, &ms5611_struct, resolution);
	  MS5611_read_press(&hi2c1, &ms5611_struct, resolutionPress);
	  MS5611_calculate(&ms5611_struct);

	  temperature = ms5611_struct.TEMP / 100.0;  // °C
	  pressure = ms5611_struct.P    / 100.0;  // hPa
	  pressure = Kalman_Update(&pressureFilter, pressure);
//	  referencePressure = pressure;                     // İlk basıncı referans al
	  currentPressure   = pressure;

	  calculateAltitude();           // altitude = 0 (çünkü ratio 1)
	  baselineAltitude = altitude;

}

void altitudeControl(){
	MS5611_read_temp(&hi2c1, &ms5611_struct, resolution);
	MS5611_read_press(&hi2c1, &ms5611_struct, resolutionPress);
	MS5611_calculate(&ms5611_struct);

	temperature = ms5611_struct.TEMP / 100;
	pressure =  ms5611_struct.P / 100.0f;
	currentPressure = pressure;
	calculateAltitude();

	yukseklik = baselineAltitude + altitude;
}
