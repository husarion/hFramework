/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_QUEUE_H
#define H_QUEUE_H

#include "hGenericQueue.h"
#include "hSystem.h"

namespace hFramework
{

/**
 * @brief Provides type oriented FIFO and LIFO queues.
 *
 * Queues are used by RTOS (Real-Time Operating System) to exchange data between different tasks.
 * @tparam T - Element type
 */
template<typename T>
class hQueue
{
public:
	/**
	 * Create queue.
	 */
	//hQueue();

	/**
	 * Initialize queue.
	 * @param length number of element in the queue
	 */
	hQueue(uint32_t length);

	/**
	 * @brief Initialize queue if constructor without parameter has been used.
	 * @param length number of element in the queue
	 */
	//void init(uint32_t length);

	/**
	 * @brief Send a single element to back (FIFO - First-In-First-Out type queue)
	 * @param Item element to be sent to queue
	 * @param timeout amount of time to wait if queue if full.
	 * @return true if element has been successfully sent to queue.
	 */
	bool sendToBack(const T& Item, uint32_t timeout = INFINITE);

	bool put(const T& item, uint32_t timeout) { return sendToBack(item, timeout); }

	/**
	 * @brief Send a single element to front (LIFO - Last-In-First-Out type queue)
	 * @param Item element to be sent to queue
	 * @param timeout amount of time to wait if queue if full.
	 * @return true if element has been successfully sent to queue.
	 */
	bool sendToFront(const T& Item, uint32_t timeout = INFINITE);

	/**
	 * @brief Remove all elements from the queue.
	 * @return true if queue is flushed successfully.
	 */
	bool flush();

	/**
	 * @brief Get a single element from the front of a queue.
	 * @param Item place where to store received element.
	 * @param timeout amount of time to wait if queue if empty.
	 * @return true if element has been successfully received from the queue.
	 */
	bool receive(T& Item, uint32_t timeout = INFINITE);

	bool get(T& item, uint32_t timeout) { return receive(item, timeout); }

	/**
	 * @brief Copy a single element from the front of a queue. After this operation queue has the same elements inside, that before.
	 * @param Item place where to store copied element.
	 * @param timeout amount of time to wait if queue if empty.
	 * @return true if element has been successfully copied from the queue.
	 */
	bool copyFromFront(T& Item, uint32_t timeout = INFINITE);

	bool peek(T& item, uint32_t timeout) { return copyFromFront(item, timeout); }

	/**
	 * @brief Get number of elements stored inside the queue.
	 * @return number of elements inside the queue.
	 */
	uint32_t getElementCnt();

	int size() { return getElementCnt(); }

	int freeSpace() { return queue.freeSpace(); }

	void clear() { queue.clear(); }

private:
	hGenericQueue queue;

	hQueue(const hQueue&) = delete;
};


template<typename T>
hQueue<T>::hQueue(uint32_t length)
	: queue(length, sizeof(T))
{
}

template<typename T>
bool hQueue<T>::sendToBack(const T& Item, uint32_t timeout)
{
	return queue.put(&Item, timeout);
}

template<typename T>
bool hQueue<T>::sendToFront(const T& Item, uint32_t timeout)
{
	return queue.putFront(&Item, timeout);
}

template<typename T>
bool hQueue<T>::flush()
{
	return queue.clear();
}

template<typename T>
bool hQueue<T>::receive(T& Item, uint32_t timeout)
{
	return queue.get(&Item, timeout);
}

template<typename T>
bool hQueue<T>::copyFromFront(T& Item, uint32_t timeout)
{
	return queue.peek(&Item, timeout);
}

template<typename T>
uint32_t hQueue<T>::getElementCnt()
{
	return queue.size();
}

}

#endif /* H_QUEUE_H */
