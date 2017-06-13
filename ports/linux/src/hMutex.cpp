/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hMutex.h>
#include <memory>

namespace hFramework {

hMutex::hMutex(MutexType type): type(type) {
    if (type == MutexType::Mutex) {
        wrapped_mutex = std::unique_ptr<std::timed_mutex>(new std::timed_mutex());
    } else {
        wrapped_rmutex = std::unique_ptr<std::recursive_timed_mutex>(new std::recursive_timed_mutex());
    }
}

bool hMutex::give() {
    if (type == MutexType::Mutex)
        wrapped_mutex->unlock();
    else
        wrapped_rmutex->unlock();
    return true;
}

bool hMutex::take(uint32_t timeout) {
    if (timeout == INFINITE) {
        if (type == MutexType::Mutex)
            wrapped_mutex->lock();
        else
            wrapped_rmutex->lock();

        return true;
    } else {
        if (type == MutexType::Mutex)
            return wrapped_mutex->try_lock_for(std::chrono::milliseconds(timeout));
        else
            return wrapped_rmutex->try_lock_for(std::chrono::milliseconds(timeout));
    }
}


}
