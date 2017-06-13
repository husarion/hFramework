/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __H_PID_REGULATOR_H__
#define __H_PID_REGULATOR_H__

#include "hRegulator.h"
#include "hPID.h"

namespace hFramework {
class hPIDRegulator : public virtual hRegulator, public hPID
{
private:
	uint8_t stableCounter;

public:
	hPIDRegulator();
	virtual ~hPIDRegulator() { }

	int32_t stableRange;
	int stableTimes;

	regFuncState regFunct(int32_t encoderNow, int32_t encoderTarget, uint16_t power, int16_t& motorOut);
};
}

#endif
