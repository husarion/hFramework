/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __PORT_H__
#define __PORT_H__

#include <hTypes.h>

#if BOARD(ROBOCORE)
#define ROBOCORE_HMOTOR_CNT  6
#define ROBOCORE_HSENSOR_CNT 5
#define ROBOCORE_HEXT_CNT    2
#define MYUSART_NUMBER_OF_USARTS 5

#elif BOARD(CORE2)
#define ROBOCORE_HMOTOR_CNT  4
#define ROBOCORE_HSENSOR_CNT 6
#define ROBOCORE_HEXT_CNT    1
#define MYUSART_NUMBER_OF_USARTS 5

#elif BOARD(CORE2MINI)
#define ROBOCORE_HMOTOR_CNT  2
#define ROBOCORE_HSENSOR_CNT 3
#define ROBOCORE_HEXT_CNT    0
#define MYUSART_NUMBER_OF_USARTS 4

#else
#  error no board
#endif

#ifdef __cplusplus
#include "peripherals.h"

namespace hFramework
{
extern hMotor *port_motors[];
}
#endif

#endif
