/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HQUEUE_H__
#define __HQUEUE_H__

#include "hMutex.h"
#include "hCondVar.h"

#include <queue>
#include <type_traits>

#include "hMutex.h"

namespace hFramework
{

template<typename T>
class hQueue
{
	static_assert(std::is_trivially_copyable<T>::value, "only trivially copyable types supported");
private:
	hMutex mutex;
	hCondVar condVarGet, condVarPut;
	std::queue<T> queue;
	uint32_t size;
public:
	hQueue() {}

	hQueue(int size)
	{
		init(size);
	}

	void init(int size)
	{
		this->size = size;
	}

	bool sendToBack(const T& Item, uint32_t timeout = 0xffffffff)
	{
		return put(Item, timeout);
	}

	bool copyFromFront(T& Item, uint32_t timeout = 0xffffffff)
	{
		return peek(Item, timeout);
	}
	
	bool receive(T& Item, uint32_t timeout = 0xffffffff)
	{
		return get(Item, timeout);
	}

	bool put(const T& data, int timeout)
	{
		mutex.lock();
		while (queue.size() >= size)
		{
			if (!condVarPut.wait(mutex, timeout))
			{
				mutex.unlock();
				return false;
			}
		}

		queue.push(data);

		mutex.unlock();
		condVarGet.notify_one();
		return true;
	}

	bool peek(T& data, int timeout)
	{
		mutex.lock();
		while (queue.empty())
		{
			if (!condVarGet.wait(mutex, timeout))
			{
				mutex.unlock();
				return false;
			}
		}

		data = queue.front();

		mutex.unlock();
		return true;
	}

	bool get(T& data, uint32_t timeout)
	{
		mutex.lock();
		while (queue.empty())
		{
			if (!condVarGet.wait(mutex, timeout))
			{
				mutex.unlock();
				return false;
			}
		}

		data = queue.front();
		queue.pop();

		mutex.unlock();
		condVarPut.notify_one();
		return true;
	}

	int getElementCnt()
	{
		hMutexGuard guard (mutex);
		return queue.size();
	}

	int freeSpace() {
		return size - getElementCnt();
	}

	void clear() {

	}
};

}

#endif
