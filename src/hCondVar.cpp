/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

// based on "Implementing Condition Variables with Semaphores" article by Andrew D. Birrell
#include <hCondVar.h>

namespace hFramework {
hCondVar::hCondVar() {
    x.give();
    s.take(0);
}

void hCondVar::notifyOne() {
    x.take();
    if (waiters > 0) {
        waiters --;
        s.give();
    }
    x.give();
}

bool hCondVar::wait(hMutex& mutex, uint32_t timeout) {
    x.take();
    waiters ++;
    x.give();

    mutex.unlock();
    bool ok = s.take(timeout);
    mutex.lock();
    return ok;
}
}
