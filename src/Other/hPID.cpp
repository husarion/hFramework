/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hPID.h"
#include "hSystem.h"

namespace hFramework
{

hPID::hPID()
	: scale(1), Kp(1), Ki(0), Kd(0), flags(PID_FLAG_I_ENABLED), isum(0), lastError(0)
{
}
hPID::hPID(float Kp, float Ki, float Kd)
	: scale(1), Kp(Kp), Ki(Ki), Kd(Kd), flags(PID_FLAG_I_ENABLED), isum(0), lastError(0)
{
}

void hPID::reset()
{
	isum = 0;
	lastError = 0;
}

void hPID::setRangeMin(float min)
{
	this->min = min;
	flags |= PID_FLAG_HAS_MIN;
}
void hPID::setRangeMax(float max)
{
	this->max = max;
	flags |= PID_FLAG_HAS_MAX;
}
void hPID::setIRangeMin(float imin)
{
	this->imin = imin;
	flags |= PID_FLAG_HAS_IMIN;
}
void hPID::setIRangeMax(float imax)
{
	this->imax = imax;
	flags |= PID_FLAG_HAS_IMAX;
}

float hPID::update(float error, int dt_ms)
{
	float curErr;
	if (flags & PID_FLAG_HAS_LAST_ERROR)
	{
		curErr = error - lastError;
	}
	else
	{
		curErr = 0;
		flags |= PID_FLAG_HAS_LAST_ERROR;
	}
	lastError = error;

	float tmpIsum = 0;
	if (Ki > 0.0f && (flags & PID_FLAG_I_ENABLED))
	{
		isum += Ki * error * dt_ms;

		if ((flags & PID_FLAG_HAS_IMAX) && isum > imax)
			isum = imax;
		else if ((flags & PID_FLAG_HAS_IMIN) && isum < imin)
			isum = imin;

		tmpIsum = isum;
	}

	float val = Kp * error + tmpIsum + Kd * curErr / dt_ms;
	val *= scale;
	// sys.log("%f %f %x\r\n", min, max, flags);
	if ((flags & PID_FLAG_HAS_MAX) && val > max)
		val = max;
	else if ((flags & PID_FLAG_HAS_MIN) && val < min)
		val = min;
	return val;
}

}
