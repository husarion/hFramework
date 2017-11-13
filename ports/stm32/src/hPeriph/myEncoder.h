/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYENCODER_H
#define MYENCODER_H

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
#if BOARD(ROBOCORE)
	MYENCODER_HMOT1,
	MYENCODER_HMOT2,
	MYENCODER_HMOT3,
	MYENCODER_HMOT4,
	MYENCODER_HMOT5,
	MYENCODER_HMOT6,
#elif BOARD(CORE2)
	MYENCODER_HMOT1,
	MYENCODER_HMOT2,
	MYENCODER_HMOT3,
	MYENCODER_HMOT4,
#elif BOARD(CORE2MINI)
	MYENCODER_HMOT1,
	MYENCODER_HMOT2,
#else
#  error no board
#endif
};
void myEncoder_setPullup(uint8_t motorNr);
void myEncoder_setPulldown(uint8_t motorNr);
void myEncoder_setPullReset(uint8_t motorNr);
void myEncoder_init(uint8_t motorNr);
void myEncoder_setPolarity(uint8_t motorNr, uint8_t polarity);
int32_t myEncoder_getCnt(uint8_t motorNr);
void myEncoder_resetCnt(uint8_t motorNr);

#ifdef __cplusplus
}
#endif

#endif /* MYENCODER_H */
