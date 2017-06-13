/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hSystem.h>
#include <hMutex.h>

#ifdef STM32F4
#include "FreeRTOS.h"
#include "semphr.h"
#else
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#endif

#define MUTEX_DEBUG
#define DEBUG_CATEGORY MUTEX
#include <debug.h>

extern "C" {
uint32_t msToTicks(uint32_t ms);
}

namespace hFramework
{

class hMutex::hMutexImplementation
{
public:
	xSemaphoreHandle mutex;
	hMutex::EType type;
};

hMutex::hMutex(EType type) : type(type)
{
	this->impl = (hMutexImplementation*)sys.malloc(sizeof(hMutexImplementation));
	switch (type)
	{
	case Mutex:
		this->impl->mutex = xSemaphoreCreateMutex();
		break;
	case RecursiveMutex:
		this->impl->mutex = xSemaphoreCreateRecursiveMutex();
		break;
	case Semaphore:
		this->impl->mutex = xSemaphoreCreateBinary();
		break;
	}
}

hMutex::~hMutex()
{
	vSemaphoreDelete(this->impl->mutex);
	sys.free(this->impl);
}

bool hMutex::give()
{
	if (type == RecursiveMutex)
		return xSemaphoreGiveRecursive(this->impl->mutex) == pdTRUE;
	else
		return xSemaphoreGive(this->impl->mutex) == pdTRUE;
}

bool hMutex::take(uint32_t timeout)
{
	if (type == RecursiveMutex)
		return xSemaphoreTakeRecursive(this->impl->mutex, msToTicks(timeout)) == pdTRUE;
	else
		return xSemaphoreTake(this->impl->mutex, msToTicks(timeout)) == pdTRUE;
}

}
