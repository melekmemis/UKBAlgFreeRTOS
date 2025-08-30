/*
 * bno055UKB.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef BNO055UKB_H_
#define BNO055UKB_H_

#ifdef __cplusplus
extern "C" {
#endif

extern volatile float yaw, pitch, roll;
extern volatile float yawRaw, pitchRaw, rollRaw;
extern volatile float accX, accY, accZ;
extern volatile float accXraw, accYraw, accZraw;
extern volatile float gyroX, gyroY, gyroZ;
extern volatile float gyroXraw, gyroYraw, gyroZraw;

void bno055_initialize(void);
void bno055ReadAngles(void);

#ifdef __cplusplus
}
#endif

#endif /* BNO055UKB_H_ */
