/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSYSTEM_H__
#define __HSYSTEM_H__
#include <cstdint>
#include <cstdlib>
#include <functional>

namespace hFramework {
constexpr int DEFAULT_STACK_SIZE = 8 * 1024;
}

#include "hSystem_rtos.h"

namespace hFramework
{
class hSystem
{
public:
    void initRTOS();
    /**
     * @brief Delay the current task.
     * @param delay milliseconds to delay
     */
    static void delay(uint32_t delayMs);

    /**
     * @brief Delay the current task.
     * @param delay microseconds to delay (must be multiply of 500)
     */
    static void delayUs(uint32_t delay);

    static void delaySync(uint32_t& refTime, uint32_t delay);

    static void* malloc(size_t size) { return ::malloc(size); }
    static void free(void* p) { return ::free(p); }
    static uint32_t getRandNr();

    static uint32_t getRefTime();

    static hTask& taskCreate(const std::function<void()>& handler, uint8_t priority = 2, uint32_t stack = DEFAULT_STACK_SIZE, const char* desc = 0);
    static hTask& taskCreate(const std::function<void()>& handler, const char* desc) {
        return taskCreate(handler, 2, DEFAULT_STACK_SIZE, desc);
    }

    static hTimer& addTimer(uint32_t timeout, TimerMode mode, TimerRun startMode, const std::function<void()>& handler);
    static uint64_t getSerialNum();

    static void reset();
};

extern hSystem sys;
}

#endif
