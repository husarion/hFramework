/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYTIMER_H_
#define MYTIMER_H_

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


uint32_t myTimerGetCnt();
void myTimerInit();

//void myTimer_int_config(uint16_t us100);
//void myTimer_waitSem(void);
//
//void myTimer_free_config(void);
//void myTimer_wait_us(uint16_t us10);
//
////void myDAC_playWav(uint8_t* data, uint32_t size);
////void myDAC_Init(void);
////uint16_t myADC_readPin(uint8_t pinNr);

#ifdef __cplusplus
}
#endif

#endif /* MYTIMER_H_ */
