/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYEXTI_H_
#define MYEXTI_H_

// #ifdef __cplusplus
// extern "C" {
// #endif
#include "hTypes.h"
#include "myGPIO.h"
#include "ycm.h"

typedef enum
{
	SEMPHR_NONE,
	SEMPHR_NORMAL,
	SEMPHR_COUNTING,
} sem_type_t;

uint8_t getEXTI_nr(uint8_t pinNr);

void myEXTI_config(uint8_t pinNr, sem_type_t semphrType, const std::function<void()>& handler = [](){});
uint8_t myEXTI_waitIntrpt(uint8_t pinNr, uint32_t timeout);
// uint8_t myEXTI_SoftIntrpt(uint8_t extiNr);

// #ifdef __cplusplus
// }
// #endif

#endif /* MYEXTI_H_ */
