/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>
#include <cstring>

extern "C" {
#ifdef STM32F4
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "port.h"
#else
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#endif
};

#define CONFIG_MAX_ROBOT_TASK_CNT 20
#define CONFIG_MAX_ROBOT_TIMER_CNT 20

extern "C" {
constexpr uint32_t ticksToMs(uint32_t ticks) {
    return ticks * 1000 / configTICK_RATE_HZ;
}

constexpr uint32_t maxMsToWait()
{
    return ticksToMs(portMAX_DELAY);
}
constexpr uint32_t maxUsToWait()
{
    return portMAX_DELAY / configTICK_RATE_HZ;
}
constexpr uint32_t tickDuraionInUs()
{
    return 1000 * 1000 / configTICK_RATE_HZ;
}


uint32_t msToTicks(uint32_t ms)
{
    if (ms == INFINITE) {
        return INFINITE;
    } else if (ms >= maxMsToWait()) {
        ASSERT2(0, "too long ms wait got 0x%08x max 0x%08x", ms, maxMsToWait());
        return 0;
    } else {
        return ms * configTICK_RATE_HZ / 1000;
    }
}
uint32_t usToTicks(uint32_t us)
{
    if (us == INFINITE)
    {
        return INFINITE;
    }
    else if (us >= maxUsToWait())
    {
        ASSERT2(0, "too long us wait got 0x%08x max 0x%08x", us, maxUsToWait());
        return 0;
    }
    else
    {
        return us * configTICK_RATE_HZ / 1000 / 1000;
    }
}
}

namespace hFramework {

class hTaskPimpl
{
public:
    std::function<void()> handler;
    bool toDestroy;
    hSemaphore taskSemaphore;
    hTaskHandle taskHandle;

    hTaskPimpl() : toDestroy(0), taskHandle(0) { }
};

class hTimerPimpl
{
public:
    std::function<void()> handler;
    TimerHandle_t timerHandle;

    bool isEmpty()
    {
        return timerHandle == 0;
    }

    hTimerPimpl() : timerHandle(0) {  }
};

hTask tasks[CONFIG_MAX_ROBOT_TASK_CNT];
hTaskPimpl tasksPimpls[CONFIG_MAX_ROBOT_TASK_CNT];
hTimer timers[CONFIG_MAX_ROBOT_TIMER_CNT];
hTimerPimpl timersPimpls[CONFIG_MAX_ROBOT_TIMER_CNT];
hMutex taskCreationMutex;

void hSystem::initRTOS() {
    for (int i = 0; i < CONFIG_MAX_ROBOT_TASK_CNT; i++)
        tasks[i].pimpl = &tasksPimpls[i];
    for (int i = 0; i < CONFIG_MAX_ROBOT_TIMER_CNT; i++)
        timers[i].pimpl = &timersPimpls[i];
}

hTask& hSystem::taskCreate(const std::function<void()>& handler, uint8_t priority, uint32_t stack, const char* desc)
{
    ASSERT (stack >= 100);
    hMutexGuard guard(taskCreationMutex);

    uint8_t i;

    char taskNameTemplate[] = "hTask1\x00";
    const char* taskName = 0;

    hTask* task = 0;

    for (i = 0; i < CONFIG_MAX_ROBOT_TASK_CNT; i++)
    {
        task = &tasks[i];
        hTaskPimpl* pimpl = task->pimpl;
        if (pimpl->toDestroy) {
            pimpl->taskHandle = NULL;
            SYSLOG("task %d is empty", i);
        }

        if (pimpl->taskHandle == NULL)
        {
            pimpl->handler = handler;
            pimpl->toDestroy = false;
            pimpl->taskSemaphore.take(0);

            if (desc)
            {
                ASSERT2(strlen(desc) < configMAX_TASK_NAME_LEN,
                        "too long task name (%s) max %d got %d", desc, configMAX_TASK_NAME_LEN, strlen(desc));
                taskName = desc;
            }
            else
            {
                int num = i + 1;
                if (num >= 10)
                {
                    taskNameTemplate[6] = (num % 10) + '0';
                    num /= 10;
                }
                taskNameTemplate[5] = num + '0';
                taskName = taskNameTemplate;
            }
            break;
        }
    }
    ASSERT2(i != CONFIG_MAX_ROBOT_TASK_CNT, "task limit reached (%d)", CONFIG_MAX_ROBOT_TASK_CNT);

    auto taskRunProc = [](void* p) {
        hTaskPimpl* pimpl = (hTaskPimpl*)p;
        pimpl->handler();
        {
            hMutexGuard guard(taskCreationMutex);
            pimpl->toDestroy = true;
            pimpl->taskSemaphore.give();
        }
        SYSLOG("task %s marked to be removed", pcTaskGetTaskName(pimpl->taskHandle));
        vTaskDelete(NULL);
    };

    // SYSLOG("creating task %d with name %s created", i, taskName);
    if (xTaskCreate(taskRunProc, taskName, stack, task->pimpl, priority, &task->pimpl->taskHandle) != pdTRUE)
        ASSERT2(0, "unable to create task %s", taskName);
    else
        SYSLOG("task %d with name %s created", i, taskName);

    return *task;
}

void hTimer::start()
{
    xTimerStart(pimpl->timerHandle, 0);
}
void hTimer::stop()
{
    xTimerStop(pimpl->timerHandle, 0);
}
bool hTimer::isRunning()
{
    return pimpl->isEmpty();
}
void hTimer::setPeriod(uint32_t period)
{
    xTimerChangePeriod(pimpl->timerHandle, msToTicks(period), 0);
}
void hTimer::free()
{
    if (pimpl->timerHandle)
        xTimerDelete(pimpl->timerHandle, msToTicks(1));
    pimpl->timerHandle = 0;
}

const char* hTask::getName()
{
    return pcTaskGetTaskName(pimpl->taskHandle);
}
bool hTask::join(uint32_t timeout)
{
    return pimpl->taskSemaphore.take(timeout);
}
bool hTask::isRunning()
{
    return pimpl->taskHandle && !pimpl->toDestroy;
}

hTimer& hSystem::addTimer(uint32_t timeout, TimerMode mode, TimerRun startMode, const std::function<void()>& handler)
{
    hMutexGuard guard(taskCreationMutex);

    hTimer *timer;
    for (int i = 0; i < CONFIG_MAX_ROBOT_TIMER_CNT; i++)
    {
        timer = &timers[i];
        hTimerPimpl* pimpl = timer->pimpl;

        if (pimpl->isEmpty())
        {
            SYSLOG("adding timer %d", i);
            timer->free();
            auto timerRunProc = [](void* timer) {
                 TimerHandle_t xTimer = (TimerHandle_t)timer;
                 hTimerPimpl* pimpl = (hTimerPimpl*)pvTimerGetTimerID(xTimer);
                 pimpl->handler();
            };
            pimpl->timerHandle = xTimerCreate("", msToTicks(timeout), mode == TimerMode::Repeat ? true : false, pimpl, timerRunProc);
            pimpl->handler = handler;

            if (startMode == TimerRun::Immediately)
                timer->start();
            return *timer;
        }
    }
    SYSLOG("no timer available");
    return timers[0];
}

void hSystem::delay(uint32_t delay)
{
    vTaskDelay(msToTicks(delay));
}
void hSystem::delayUs(uint32_t delay)
{
    uint32_t step = tickDuraionInUs();
    delay = ((delay + step - 1) / step) * step; // rounding to multiply of 500 (tickDuration)
    vTaskDelay(usToTicks(delay));
}

void hSystem::delaySync(uint32_t& refTime, uint32_t delay)
{
    uint32_t refticks = refTime * configTICK_RATE_HZ / 1000;
    vTaskDelayUntil((portTickType*)&refticks, msToTicks(delay));
    refTime = refticks * configTICK_RATE_HZ / 1000;
}

uint32_t hSystem::getRefTime()
{
    return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}

}
