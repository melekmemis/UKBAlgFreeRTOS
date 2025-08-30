/*
 * ms5611UKB.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef MS5611UKB_H_
#define MS5611UKB_H_

#ifdef __cplusplus
extern "C" {
#endif

extern float yukseklik;
extern volatile double currentPressure, altitude;
extern volatile double temperature;
extern volatile double pressure;

void altitudeControl();
void MS5611_Start();

#ifdef __cplusplus
}
#endif

#endif /* MS5611UKB_H_ */
