/*
 * testDevice.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "testDevice.h"
#include "bno055UKB.h"
#include "ms5611UKB.h"
#include "flightStatus.h"

uint8_t statusPacket[6];
uint8_t statusPackData1;
uint8_t bytes[4];
uint8_t firstTimeSut;
int firstTime = 1;
float gyronf;

uint8_t rxDataReceived[rxDataSize];
uint8_t command[commandSize];
uint8_t ssensorData[sensorDataSize];
uint8_t commandCRC;
uint8_t CRCconstant = 0x3E;

int recievedSize;
int isRight = 0;

commandCase cmd;
testMode mode;

uint8_t testCihazBuffer[36];

uint8_t checkSumCalc2(){
    int check_sum = 0;

    for(int i = 0; i < 33; i++){
        check_sum += testCihazBuffer[i];
    }

    return (uint8_t) (check_sum % 256);
}

//uint8_t checkSumCalc(void *data, size_t size){
//    int checkSum = 0;
//
//    uint8_t buffer[size];
//    memcpy(buffer, data, size);
//
//    for(int i = 0; i < size; i++){
//    	checkSum += buffer[i];
//    }
//
//    return (uint8_t) (checkSum % 256);
//}

commandCase parseCommand(uint8_t *data){
	if (data[0] != 0xAA)
		return invalidCommand;

	switch (data[1]){
		case 0x20:
			commandCRC = data[0] + data[1] - CRCconstant;

			if ((0xCA == data[2]) && (data[3] == 0x0D) && (data[4] == 0x0A))
				{ return sitCommand; }
			else
				return invalidCommand;
		break;
		case 0x22:
			commandCRC = data[0] + data[1] - CRCconstant;

			if ((0xCC == data[2]) && (data[3] == 0x0D) && (data[4] == 0x0A))
				{firstTimeSut = 1; return sutCommand;}
			else
				return invalidCommand;
		break;
		case 0x24:
			commandCRC = data[0] + data[1] - CRCconstant;

			if ((0xCE == data[2]) && (data[3] == 0x0D) && (data[4] == 0x0A))
				return stopCommand;
			else
				return invalidCommand;
		break;
	}

	return invalidCommand;
}

uint8_t floatBytes[4];

typedef union {
    float sayi_f32;
    uint8_t array[4];
} FLOAT32_UINT8_DONUSTURUCU;

float uint8_arrayi_float32_ye_donustur(uint8_t byte_array_u8[4]) {
    FLOAT32_UINT8_DONUSTURUCU float32_uint8_donusturucu;
    float32_uint8_donusturucu.array[0] = byte_array_u8[3];
    float32_uint8_donusturucu.array[1] = byte_array_u8[2];
    float32_uint8_donusturucu.array[2] = byte_array_u8[1];
    float32_uint8_donusturucu.array[3] = byte_array_u8[0];
    return float32_uint8_donusturucu.sayi_f32;
}

volatile uint32_t prev_time = 0;
volatile uint32_t current_time = 0;
volatile uint32_t dt = 0;

void float_to_bytes(float value, uint8_t* out_bytes) {
    union {
        float f;
        uint8_t b[4];
    } converter;

    converter.f = value;

        out_bytes[0] = converter.b[3];
        out_bytes[1] = converter.b[2];
        out_bytes[2] = converter.b[1];
        out_bytes[3] = converter.b[0];
}

void modeCheck() {

	if(mode == normalMode){
		switch(cmd){
			case sitCommand:
				osDelay(1000);
				mode = sitMode;
			break;
			case sutCommand:
				mode = sutMode;
				accX = 0, accY = 0, accZ = 0, gyroX = 0, gyroY = 0, gyroZ = 0;
				liftOff = 0, burnOut = 0, altThrs = 0, angleCond = 0, altCond = 0, dragStep = 0, secAltCond = 0, mainStep = 0;
				statusPackData1 = 0x00;
//				fsSayac = 0;
			break;
			default:
				mode = normalMode;
		}
	}

	if(mode == sitMode){
		switch(cmd){
		case sutCommand:
			mode = sutMode;
			accX = 0, accY = 0, accZ = 0, gyroX = 0, gyroY = 0, gyroZ = 0;
			liftOff = 0, burnOut = 0, altThrs = 0, angleCond = 0, altCond = 0, dragStep = 0, secAltCond = 0, mainStep = 0;
			statusPackData1 = 0x00;
//			fsSayac = 0;
		break;
		case stopCommand:
			mode = normalMode;
		break;
		default:
			mode = sitMode;
		}
	}

	if(mode == sutMode){
		if (firstTimeSut) {
			accX = 0, accY = 0, accZ = 0, gyroX = 0, gyroY = 0, gyroZ = 0, altitude = 0;
			firstTimeSut = 0;
			liftOff = 0, burnOut = 0, altThrs = 0, angleCond = 0, altCond = 0, dragStep = 0, secAltCond = 0, mainStep = 0;
			statusPackData1 = 0x00;
		}
		if(recievedSize == sensorDataSize) {
			floatBytes[0] = rxDataReceived[1];
			floatBytes[1] = rxDataReceived[2];
			floatBytes[2] = rxDataReceived[3];
			floatBytes[3] = rxDataReceived[4];
			altitude = uint8_arrayi_float32_ye_donustur(floatBytes);

			floatBytes[0] = rxDataReceived[5];
			floatBytes[1] = rxDataReceived[6];
			floatBytes[2] = rxDataReceived[7];
			floatBytes[3] = rxDataReceived[8];
			pressure = uint8_arrayi_float32_ye_donustur(floatBytes);

			floatBytes[0] = rxDataReceived[9];
			floatBytes[1] = rxDataReceived[10];
			floatBytes[2] = rxDataReceived[11];
			floatBytes[3] = rxDataReceived[12];
			accX = uint8_arrayi_float32_ye_donustur(floatBytes)/9.81f;

			floatBytes[0] = rxDataReceived[13];
			floatBytes[1] = rxDataReceived[14];
			floatBytes[2] = rxDataReceived[15];
			floatBytes[3] = rxDataReceived[16];
			accY = uint8_arrayi_float32_ye_donustur(floatBytes)/9.81f;

			floatBytes[0] = rxDataReceived[17];
			floatBytes[1] = rxDataReceived[18];
			floatBytes[2] = rxDataReceived[19];
			floatBytes[3] = rxDataReceived[20];
			accZ = uint8_arrayi_float32_ye_donustur(floatBytes)/9.81f;

			floatBytes[0] = rxDataReceived[21];
			floatBytes[1] = rxDataReceived[22];
			floatBytes[2] = rxDataReceived[23];
			floatBytes[3] = rxDataReceived[24];
			pitch = uint8_arrayi_float32_ye_donustur(floatBytes);


			floatBytes[0] = rxDataReceived[25];
			floatBytes[1] = rxDataReceived[26];
			floatBytes[2] = rxDataReceived[27];
			floatBytes[3] = rxDataReceived[28];
			roll = uint8_arrayi_float32_ye_donustur(floatBytes);

			floatBytes[0] = rxDataReceived[29];
			floatBytes[1] = rxDataReceived[30];
			floatBytes[2] = rxDataReceived[31];
			floatBytes[3] = rxDataReceived[32];
			yaw = uint8_arrayi_float32_ye_donustur(floatBytes);

		}

		switch(cmd){
		case sitCommand:
			mode = sitMode;
		break;
		case stopCommand:
			mode = normalMode;
			yaw = 0, pitch = 0, roll = 0;
			liftOff = 0, burnOut = 0, altThrs = 0, angleCond = 0, altCond = 0, dragStep = 0, secAltCond = 0, mainStep = 0;
			statusPackData1 = 0x00;
		break;
		default:
			mode = sutMode;
		}

	}
}
