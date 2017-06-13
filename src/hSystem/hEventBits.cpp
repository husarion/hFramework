/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSystem.h"
#include "hEventBits.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

namespace hFramework
{

///////////////////// hEventBits implementation class /////////////////////////////////


class hEventBits::hEventBitsImplementation
{
public:
	EventGroupHandle_t eventGroupHandle;
};



///////////////////// hEventBits /////////////////////////////////

hEventBits::hEventBits()
{
	this->impl = (hEventBitsImplementation *) sys.malloc(sizeof(hEventBitsImplementation));
	this->impl->eventGroupHandle = xEventGroupCreate();
}

hEventBits::~hEventBits()
{
	vEventGroupDelete(this->impl->eventGroupHandle);
	sys.free(this->impl);
}
uint32_t hEventBits::waitAll(uint32_t bitMask, uint32_t timeout)
{
	return (uint32_t)xEventGroupWaitBits(this->impl->eventGroupHandle, (const EventBits_t)(bitMask & 0x00FFFFFF), pdFALSE, pdTRUE, (TickType_t)timeout);
}

uint32_t hEventBits::waitAny(uint32_t bitMask, uint32_t timeout)
{
	return (uint32_t)xEventGroupWaitBits(this->impl->eventGroupHandle, (const EventBits_t)(bitMask & 0x00FFFFFF), pdFALSE, pdFALSE, (TickType_t)timeout);
}

uint32_t hEventBits::setBits(uint32_t bitsToSet)
{
	return (uint32_t)xEventGroupSetBits(this->impl->eventGroupHandle, (const EventBits_t)(bitsToSet & 0x00FFFFFF));
}

uint32_t hEventBits::clearBits(uint32_t bitsToClear)
{
	return (uint32_t)xEventGroupClearBits(this->impl->eventGroupHandle, (const EventBits_t)(bitsToClear & 0x00FFFFFF));
}

uint32_t hEventBits::clear()
{
	return (uint32_t)xEventGroupClearBits(this->impl->eventGroupHandle, (const EventBits_t)0x00FFFFFF);
}

uint32_t hEventBits::get()
{
	return (uint32_t)xEventGroupGetBits(this->impl->eventGroupHandle);
}

}
