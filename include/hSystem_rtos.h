/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

namespace hFramework {

/**
 * @brief Speficies timer mode
 */
enum class TimerMode
{
    OneShot, //!< Timer handler would run only once.
    Repeat //!< Timer handler would be run at specific interval.
};

/**
 * @brief Speficies start mode
 */
enum class TimerRun
{
    Immediately, //!< Timer will start immediately.
        Wait //!< Timer will not be started, a explicit call to hTimer::start would be needed.
};

typedef void* hTaskHandle;

class hTaskPimpl;
class hTimerPimpl;

class hTask
{
public:
    const char* getName();
    bool join(uint32_t timeout = 0xffffffff);
    bool isRunning();

private:
    hTaskPimpl* pimpl;

    friend class hSystem;
};

class hTimer
{
public:
    /**
     * @brief Start timer.
     */
    void start();
    void stop();
    bool isRunning();
    void setPeriod(uint32_t period);
    void free();

private:
    hTimerPimpl* pimpl;

    friend class hSystem;
};
}
