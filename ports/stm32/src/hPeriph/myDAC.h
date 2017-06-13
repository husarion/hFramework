/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYDAC_H_
#define MYDAC_H_

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void myDAC_config(uint8_t pinNr);
void myDAC_playWav(uint8_t* data, uint32_t size);
void myDAC_Init(void);
//uint16_t myADC_readPin(uint8_t pinNr);

#ifdef __cplusplus
}
#endif

#endif /* MYDAC_H_ */
