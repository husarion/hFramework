/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HELAPSEDTIMER_H__
#define __HELAPSEDTIMER_H__

#include <hFramework.h>

namespace hFramework
{

class hElapsedTimer
{
private:
	uint32_t interval;
	uint32_t lastExecTime;
	uint32_t elapsedTimes;

public:
	hElapsedTimer() : interval(0), lastExecTime(0), elapsedTimes(0)
	{

	}
	hElapsedTimer(uint32_t intervalMs)
		: interval(intervalMs), elapsedTimes(0)
	{
		lastExecTime = sys.getRefTime();
	}
	hElapsedTimer(uint32_t intervalMs, uint32_t firstRunDelayMs)
		: interval(intervalMs), elapsedTimes(0)
	{
		lastExecTime = sys.getRefTime() - interval;
	}

	bool hasElapsed()
	{
		uint32_t time = sys.getRefTime();

		if (time - lastExecTime >= interval)
		{
			lastExecTime = time;
			elapsedTimes++;
			return true;
		}
		else
		{
			return false;
		}
	}

	uint32_t getElapsedTimes() const { return elapsedTimes; }
};

}

#endif
