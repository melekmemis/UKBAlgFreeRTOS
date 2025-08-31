/*
 * sdCard.h
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int dataSize;
extern int yazdi;

void sdCardOpen();
void sdCardClose();
void sdCardWrite();

#ifdef __cplusplus
}
#endif

#endif /* SDCARD_H_ */
