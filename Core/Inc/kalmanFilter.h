/*
 * kalmanFilter.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef KALMANFILTER_H_
#define KALMANFILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float q;
    float r;
    float x;
    float p;
    float k;
} KalmanFilter;

extern KalmanFilter accXFilter, accYFilter, accZFilter;
extern KalmanFilter gyroXFilter, gyroYFilter, gyroZFilter;
extern KalmanFilter pitchFilter, rollFilter, yawFilter;
extern KalmanFilter pressureFilter, altitudeFilter;

void KalmanInit(KalmanFilter *kf, float q, float r, float initialvalue);
float KalmanUpdate(KalmanFilter *kf, float measurement);
void KalmanInitAll(void);

#ifdef __cplusplus
}
#endif

#endif /* KALMANFILTER_H_ */
