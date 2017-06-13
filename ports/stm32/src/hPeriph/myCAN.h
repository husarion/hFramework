/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYCAN_H_
#define MYCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hTypes.h"

void myCAN_init();
void myCAN_sendFrame(CAN_frameTx* frame);
uint8_t myCAN_waitFrame(CAN_frameRx* frame, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* MYCAN_H_ */
