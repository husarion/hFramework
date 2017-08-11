/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <condition_variable>

#include <hCondVar.h>
#include <hSystem.h>

#include <mutex>

namespace hFramework
{

void hCondVar::notifyOne()
{
	d_condition.notify_one();
}
bool hCondVar::wait(hMutex& mutex, uint32_t timeout)
{
	if (timeout == 0xffffffff) {
		if (mutex.wrapped_mutex)
			d_condition.wait(*mutex.wrapped_mutex);
		else
			d_condition.wait(*mutex.wrapped_rmutex);
		return true;
	} else {
		uint64_t deadline = timeout + sys.getRefTime();
		while (true) {
			bool ret;
			if (mutex.wrapped_mutex)
				ret = d_condition.wait_for(*mutex.wrapped_mutex, std::chrono::milliseconds(timeout)) == std::cv_status::no_timeout;
			else
				ret = d_condition.wait_for(*mutex.wrapped_rmutex, std::chrono::milliseconds(timeout)) == std::cv_status::no_timeout;

			uint64_t currTime = sys.getRefTime(); // probably spurious wakeup!
			if (!ret && currTime < deadline) {
				timeout = deadline - currTime;
				continue;
			}
			return ret;
		}
	}
}

}
