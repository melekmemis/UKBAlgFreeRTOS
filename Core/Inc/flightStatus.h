/*
 * flightStatus.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef FLIGHTSTATUS_H_
#define FLIGHTSTATUS_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int liftOff, burnOut, altThrs, angleCond,
    altCond, dragStep, secAltCond, mainStep;

void checkFlightStatus();

#ifdef __cplusplus
}
#endif

#endif /* FLIGHTSTATUS_H_ */
