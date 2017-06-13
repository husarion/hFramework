/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>

//FreeRTOS

#ifdef STM32F4
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#else
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#endif

extern "C" {
uint32_t msToTicks(uint32_t ms);
}

namespace hFramework
{

class hGenericQueue::hGenericQueuePimpl
{
public:
	xQueueHandle queue;
	uint32_t maxSize;
};

hGenericQueue::hGenericQueue(uint32_t maxSize, uint32_t itemSize)
{
	this->impl = new hGenericQueuePimpl();
	this->impl->queue = xQueueCreate(maxSize, itemSize);
	this->impl->maxSize = maxSize;
}

hGenericQueue::~hGenericQueue()
{
	if (impl->queue)
		vQueueDelete(impl->queue);
	delete impl;
}

bool hGenericQueue::put(const void* item, uint32_t timeout)
{
	return xQueueSendToBack(impl->queue, item, msToTicks(timeout)) == pdTRUE;
}

bool hGenericQueue::putFront(const void* item, uint32_t timeout)
{
	return xQueueSendToFront(impl->queue, item, msToTicks(timeout)) == pdTRUE;
}

#ifdef STM32F4
bool hGenericQueue::putFromISR(const void* item, long* xHigherPriorityTaskWoken)
{
	return xQueueSendToBackFromISR(impl->queue, item, xHigherPriorityTaskWoken) == pdTRUE;
}
#endif

bool hGenericQueue::get(void* item, uint32_t timeout)
{
	return xQueueReceive(impl->queue, item, msToTicks(timeout)) == pdTRUE;
}

bool hGenericQueue::peek(void* item, uint32_t timeout)
{
	return xQueuePeek(impl->queue, item, msToTicks(timeout)) == pdTRUE;
}

uint32_t hGenericQueue::size()
{
	return uxQueueMessagesWaiting(impl->queue);
}

uint32_t hGenericQueue::freeSpace()
{
	return impl->maxSize - size();
}

bool hGenericQueue::clear()
{
	return xQueueReset(impl->queue) == pdTRUE;
}

}
