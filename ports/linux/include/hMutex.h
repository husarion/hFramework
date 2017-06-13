/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HMUTEX_H__
#define __HMUTEX_H__

#include <mutex>
#include <memory>
#include <IMutex.h>
#include <hTypes.h>

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
	hMutex(MutexType type = MutexType::Mutex); //!< Initialize mutex.
	virtual ~hMutex() {} //!< Destroy mutex.

	bool give() override;
	bool take(uint32_t timeout = INFINITE) override;

private:
	std::unique_ptr<std::timed_mutex> wrapped_mutex;
	std::unique_ptr<std::recursive_timed_mutex> wrapped_rmutex;
	MutexType type;

	hMutex(const hMutex&) = delete;

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
	hRecursiveMutex() : hMutex(MutexType::RecursiveMutex) { } //!< Initialize recursive mutex.
	~hRecursiveMutex() { } //!< Destroy mutex.
};

/**
 * @brief Semaphore class.
 *
 * Semaphores are used by RTOS (Real-Time Operating System) to provide task synchronization.
 */
class hSemaphore : public hMutex
{
public:
	hSemaphore() : hMutex(MutexType::Semaphore) { } //!< Initialize semaphore.
	~hSemaphore() { } //!< Destroy semaphore.

private:
	hSemaphore(const hSemaphore&) = delete;
};

}


#endif /* __HMUTEX_H__ */
