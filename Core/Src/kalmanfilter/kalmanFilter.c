/*
 * kalmanFilter.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "kalmanFilter.h"

KalmanFilter accXFilter, accYFilter, accZFilter;
KalmanFilter gyroXFilter, gyroYFilter, gyroZFilter;
KalmanFilter pitchFilter, rollFilter, yawFilter;
KalmanFilter pressureFilter, altitudeFilter;

void Kalman_Init(KalmanFilter *kf, float q, float r, float initial_value) {
    kf->q = q;
    kf->r = r;
    kf->x = initial_value;
    kf->p = 1.0f;
    kf->k = 0.0f;
}

float Kalman_Update(KalmanFilter *kf, float measurement) {
    kf->p = kf->p + kf->q;

    kf->k = kf->p / (kf->p + kf->r);

    kf->x = kf->x + kf->k * (measurement - kf->x);

    kf->p = (1.0f - kf->k) * kf->p;

    return kf->x;
}

void Kalman_Init_All(void) {
    Kalman_Init(&accXFilter, 0.02f, 0.2f, 0.0f);
    Kalman_Init(&accYFilter, 0.02f, 0.2f, 0.0f);
    Kalman_Init(&accZFilter, 0.02f, 0.2f, 0.0f);

    Kalman_Init(&gyroXFilter, 0.02f, 0.2f, 0.0f);
    Kalman_Init(&gyroYFilter, 0.02f, 0.2f, 0.0f);
    Kalman_Init(&gyroZFilter, 0.02f, 0.2f, 0.0f);

    Kalman_Init(&pitchFilter, 0.04f, 0.4f, 0.0f);
    Kalman_Init(&rollFilter, 0.04f, 0.4f, 0.0f);
    Kalman_Init(&yawFilter, 0.04f, 0.4f, 0.0f);

    Kalman_Init(&pressureFilter, 0.005f, 0.05f, 1013.25f);
    Kalman_Init(&altitudeFilter, 0.005f, 0.05f, 0.0f);
}
