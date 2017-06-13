/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hPIDRegulator.h>
#include <hUtils.h>
#include <debug.h>

namespace hFramework {
hPIDRegulator::hPIDRegulator() : stableCounter(0)
{
}

regFuncState hPIDRegulator::regFunct(int32_t encoderNow, int32_t encoderTarget, uint16_t power, int16_t& motorOut)
{
	int32_t temp_motorOut;

	ASSERT(power <= 1000);

	setRange(power);
	setIRange(power);

	int32_t err = encoderTarget - encoderNow;

	if (err < 50 && err > -50)
		enableI();
	else
		disableI();

	float v = update(err, dtMs);

	temp_motorOut = (int32_t)v;

	motorOut = (int16_t)temp_motorOut;

	if ((err < stableRange) && (err > -stableRange))
	{
		stableCounter++;
		if (stableCounter == stableTimes)
		{
			motorOut = 0;
			stableCounter = 0;
			return REG_FINISH_CONTINUE;
		}
	}
	else
	{
		stableCounter = 0;
	}
	return REG_IN_PROGRESS;
}
}
