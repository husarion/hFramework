/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HMUTEX_H__
#define __HMUTEX_H__

#include <hTypes.h>
#include <IMutex.h>

namespace hFramework
{

/**
 * @brief Mutex class.
 *
 * Mutexes are used by Operating System to provide task synchronization.
 */
class hMutex : public IMutex
{
public:
	enum EType { Mutex, RecursiveMutex, Semaphore };

	hMutex(EType type = Mutex); //!< Initialize mutex.
	virtual ~hMutex(); //!< Destroy mutex.

	/**
	 * @brief Give mutex to blocked tasks.
	 *
	 * If mutex isn't necessary it can be given to blocked tasks.
	 * @return true if Mutex is ready to be taken.
	 */
	bool give();
	bool unlock() { return give(); }

	/**
	 * @brief Try to take a mutex and block a task until another task makes the mutex available.
	 * @param timeout amount of time to wait for a mutex, in ms. INFINITE to wait indefinitely.
	 * @return true if Mutex has been successfully taken before timeout, false otherwise.
	 */
	bool take(uint32_t timeout = INFINITE);
	bool lock(uint32_t timeout = INFINITE) { return take(timeout); }

private:
	class hMutexImplementation;
	hMutexImplementation *impl;
	EType type;

	hMutex(const hMutex&) = delete;

	void* getMutex();

	friend class hCondVar;
};

/**
 * @brief Recursive mutex class.
 *
 * Mutexes are used by RTOS (Real-Time Operating System) to provide task synchronization.
 */
class hRecursiveMutex : public hMutex
{
public:
	hRecursiveMutex() : hMutex(RecursiveMutex) { } //!< Initialize recursive mutex.
	~hRecursiveMutex() { } //!< Destroy mutex.
};

class hSemaphore : public hMutex
{
public:
	hSemaphore() : hMutex(Semaphore) { } //!< Initialize recursive mutex.
	~hSemaphore() { } //!< Destroy mutex.
};

}

#endif /* __HMUTEX_H__ */
