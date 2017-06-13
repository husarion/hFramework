/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hCAN.h"

#ifdef CAN_ENABLED

#include <hGPIO.h>

#include <myCAN.h>
#include <myGPIO.h>

#include <FreeRTOS.h>

namespace hFramework
{

hCAN::hCAN() : pinEnable(MYGPIO_CAN_EN)
{
}

void hCAN::init()
{
	pinEnable.init();
	pinEnable.setOut();
	enable();
}

void hCAN::sendFrame(CAN_frameTx& frame)
{
	myCAN_sendFrame(&frame);
}

void hCAN::enable()
{
#if BOARD(ROBOCORE)
	pinEnable.write(1);
#elif BOARD(CORE2)
	pinEnable.write(0);
#endif
	myCAN_init();
}
void hCAN::disable()
{
#if BOARD(ROBOCORE)
	pinEnable.write(0);
#elif BOARD(CORE2)
	pinEnable.write(1);
#endif
}

bool hCAN::waitFrame(CAN_frameRx& frame, uint32_t timeout)
{
	return myCAN_waitFrame(&frame, timeout);
}

}

#endif
