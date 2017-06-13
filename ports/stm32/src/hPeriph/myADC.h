/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __MYADC_H__
#define __MYADC_H__

#include "hTypes.h"

void myADC_init();
void myADC_config(uint8_t pinNr);
uint16_t myADC_readPin(uint8_t pinNr);

#endif /* __MYADC_H__ */
