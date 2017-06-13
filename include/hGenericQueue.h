/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HGENERICQUEUE_H__
#define __HGENERICQUEUE_H__

#include "hTypes.h"
namespace hFramework
{
/**
 * @brief Provides generic, byte oriented FIFO and LIFO queues.
 */
class hGenericQueue
{
public:
	hGenericQueue(uint32_t maxSize, uint32_t itemSize);

	~hGenericQueue();

	bool put(const void* item, uint32_t timeout = 0xffffffff);
	
	bool putFront(const void* item, uint32_t timeout = 0xffffffff);

	#ifdef STM32F4
	bool putFromISR(const void* item, long* xHigherPriorityTaskWoken);
	#endif

	bool get(void* item, uint32_t timeout = 0xffffffff);

	bool peek(void* item, uint32_t timeout = 0xffffffff);

	uint32_t size();

	uint32_t freeSpace();

	bool clear();

private:
	class hGenericQueuePimpl;
	hGenericQueuePimpl *impl;
	
	hGenericQueue(const hGenericQueue&) = delete;
};

}
#endif
