/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "IServo.h"

namespace hFramework
{

// ***************************************************************
// *************************** IServo ****************************
// ***************************************************************
IServo::IServo() : calibrateDone(false)
{
}
IServo::~IServo()
{
}
void IServo::calibrate(int16_t angleMin, uint16_t width_us_min, int16_t angleMax, uint16_t width_us_max)
{
	this->angleMin = angleMin;
	this->width_us_min = width_us_min;
	this->angleMax = angleMax;
	this->width_us_max = width_us_max;
	calibrateDone = true;
}
void IServo::rotAbs(int16_t angle)
{
	if (calibrateDone)
	{
		angle -= angleMin;
		int width = width_us_min + ((angle * (width_us_max - width_us_min)) / (angleMax - angleMin));
		setWidth(width);
	}
}

}
