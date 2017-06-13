/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hButton.h"

#include <hSystem.h>
#include <system.h>
#include "myGPIO.h"
#include "FreeRTOS.h"
#include "timers.h"

namespace hFramework
{

int hButtonDefs[] =
{
#if BOARD(ROBOCORE)
	MYGPIO_BUTTON_1,
	MYGPIO_BUTTON_2,
#elif BOARD(CORE2)
	MYGPIO_BUTTON_1,
	MYGPIO_BUTTON_2,
	MYGPIO_RPI_BTN,
#elif BOARD(CORE2MINI)
	MYGPIO_BUTTON_1,
	MYGPIO_RPI_BTN,
#else
#  error no board
#endif
};

// hButton_int
void hButton_int::userHandler(void* timer)
{
	TimerHandle_t xTimer = (TimerHandle_t)timer;
	hButton_int* btn = (hButton_int*)pvTimerGetTimerID(xTimer);

	if (btn->pin.read() == 0)
		btn->pressHandler();
	else
		btn->releaseHandler();
}

hButton_int::hButton_int(hButton_ID id)
	: pin(hButtonDefs[id]), pressHandler(0), releaseHandler(0)
{
}

void hButton_int::init()
{
	pin.init();
	pin.setIn();

	timer = xTimerCreate("", msToTicks(10), pdFALSE, this, userHandler);

	pin.interruptOn(InterruptEdge::Both, [this]()
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xTimerStartFromISR(timer, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	});
}

bool hButton_int::isPressed()
{
	return pin.read() == 0 ? true : false;
}

bool hButton_int::waitForPressed(uint32_t timeout)
{
	uint32_t s = sys.getRefTime();
	if (isPressed())
		if (!waitForReleased(timeout))
			return false;
	uint32_t elapsed = sys.getRefTime() - s;
	if (timeout == INFINITE)
		return pin.interruptWait(INFINITE);
	else
		return pin.interruptWait(timeout - elapsed);
}
bool hButton_int::waitForReleased(uint32_t timeout)
{
	uint32_t s = sys.getRefTime();
	if (!isPressed())
		if (!waitForPressed(timeout))
			return false;
	uint32_t elapsed = sys.getRefTime() - s;
	if (timeout == INFINITE)
		return pin.interruptWait(INFINITE);
	else
		return pin.interruptWait(timeout - elapsed);
}

// hButton
hButton::hButton(hButton_ID id)
	: pin(hButtonDefs[id])
{
}

void hButton::init()
{
	pin.init();
	pin.setIn();
}

bool hButton::isPressed()
{
	return pin.read() == 0 ? true : false;
}

}
