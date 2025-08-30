/*
 * testDevice.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef TESTDEVICE_H_
#define TESTDEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define commandSize 5
#define sensorDataSize 36
#define rxDataSize 50

extern uint8_t statusPacket[6];
extern uint8_t statusPackData1;
extern uint8_t firstTimeSut;
extern uint8_t testCihazBuffer[36];

typedef enum {
	noCommand, sitCommand, sutCommand, stopCommand, invalidCommand
} commandCase;

typedef enum {
	normalMode, sitMode, sutMode
} testMode;

extern int recievedSize;
extern commandCase cmd;
extern testMode mode;
extern uint8_t command[commandSize];
extern uint8_t rxDataReceived[rxDataSize];
extern uint8_t bytes[4];

commandCase parseCommand(uint8_t *data);
void modeCheck();
uint8_t checkSumCalc2();
void float_to_bytes(float value, uint8_t* out_bytes);

#ifdef __cplusplus
}
#endif

#endif /* TESTDEVICE_H_ */
