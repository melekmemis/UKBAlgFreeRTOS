/*
 * flightStatus.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "flightStatus.h"
#include "loraLib.h"

extern float accZ, pitch, roll;
extern double altitude;
extern uint8_t statusPackData1;

int liftOff, burnOut, altThrs, angleCond,
    altCond, dragStep, secAltCond, mainStep;

uint8_t durum;

#define burnOutSayac 2
uint8_t burnSayac;
float lastAcc = 0;

#define yukSayac 3
uint8_t dusSayac;
float lastAltitude = 0;

void checkFlightStatus() {

	if(accZ >= 1.6) {
		liftOff = 1;
		statusPackData1 |= (1 << 0);
	}

	if (accZ < lastAcc - 0.5) {
		burnSayac++;
	    if (burnSayac >= burnOutSayac) {
	    	burnSayac = 0;
	    	burnOut = 1;
	    	statusPackData1 |= (1 << 1);
	    }
	} else burnSayac = 0;

	lastAcc = accZ;

	if(altitude < -20) {
		altThrs = 1;
		statusPackData1 |= (1 << 2);
	}

	if((pitch > 45 || pitch < -45 || roll > 45 || roll < -45) && altThrs) {
		angleCond = 1;
		statusPackData1 |= (1 << 3);
	}

	if (altitude > lastAltitude && altThrs) {
	    dusSayac++;
	    if (dusSayac >= yukSayac) {
	    	altCond = 1;
	    	statusPackData1 |= (1 << 4);
	      dusSayac = 0;
	    }
	} else dusSayac = 0;
	lastAltitude = altitude;

	if (altCond && angleCond && !dragStep) {
		dragStep = 1;
		statusPackData1 |= (1 << 5);
		durum = 2;
		HAL_GPIO_WritePin(drogueOut_GPIO_Port, drogueOut_Pin, 1);
		osDelay(800);
		HAL_GPIO_WritePin(drogueOut_GPIO_Port, drogueOut_Pin, 0);
	}

	if (altitude < 600 && dragStep && !mainStep) {
		secAltCond = 1;
		mainStep = 1;
		statusPackData1 |= (1 << 6);
		statusPackData1 |= (1 << 7);
		durum = 4;
		HAL_GPIO_WritePin(mainOut_GPIO_Port, mainOut_Pin, 1);
		osDelay(800);
		HAL_GPIO_WritePin(mainOut_GPIO_Port, mainOut_Pin, 0);
	}
}
