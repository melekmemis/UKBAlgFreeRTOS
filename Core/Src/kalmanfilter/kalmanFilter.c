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

void KalmanInit(KalmanFilter *kf, float q, float r, float initialValue) {
    kf->q = q;
    kf->r = r;
    kf->x = initialValue;
    kf->p = 1.0f;
    kf->k = 0.0f;
}

float KalmanUpdate(KalmanFilter *kf, float measurement) {
    kf->p = kf->p + kf->q;

    kf->k = kf->p / (kf->p + kf->r);

    kf->x = kf->x + kf->k * (measurement - kf->x);

    kf->p = (1.0f - kf->k) * kf->p;

    return kf->x;
}

void KalmanInitAll(void) {
    KalmanInit(&accXFilter, 0.02f, 0.2f, 0.0f);
    KalmanInit(&accYFilter, 0.02f, 0.2f, 0.0f);
    KalmanInit(&accZFilter, 0.02f, 0.2f, 0.0f);

    KalmanInit(&gyroXFilter, 0.02f, 0.2f, 0.0f);
    KalmanInit(&gyroYFilter, 0.02f, 0.2f, 0.0f);
    KalmanInit(&gyroZFilter, 0.02f, 0.2f, 0.0f);

    KalmanInit(&pitchFilter, 0.04f, 0.4f, 0.0f);
    KalmanInit(&rollFilter, 0.04f, 0.4f, 0.0f);
    KalmanInit(&yawFilter, 0.04f, 0.4f, 0.0f);

    KalmanInit(&pressureFilter, 0.005f, 0.05f, 1013.25f);
    KalmanInit(&altitudeFilter, 0.005f, 0.05f, 0.0f);
}
