/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ISERVO_H__
#define __ISERVO_H__

#include <hTypes.h>

namespace hFramework
{

/**
 * @brief Provides interface to servo.
 */
class IServo
{
public:
	IServo();
	virtual ~IServo();

	/**
	 * @brief Set duration of positive pulse driving the servo.
	 * @param width_us duration of the pulse in microseconds.
	 */
	virtual void setWidth(uint16_t width_us) = 0;

	uint16_t getWidth() const { return curWidth; }

	/**
	 * @brief Set period of the PWM.
	 * @param period_us period of the pulses in microseconds.
	 */
	virtual void setPeriod(uint16_t period_us) = 0;

	/**
	 * @brief Set calibration coefficients.
	 *
	 * Different servos has different pulse lengths, that correspond to the actual value. With this function
	 * you can calibrate servo to use rotAbs function that acts the same way in each servo.
	 *
	 * @param angleMin minimal angle that is available for the servo.
	 * @param width_us_min pulse width in microseconds that corresponds to the angleMin.
	 * @param angleMax maximal angle that is available for the servo.
	 * @param width_us_max pulse width in microseconds that corresponds to the angleMax.
	 */
	void calibrate(int16_t angleMin, uint16_t width_us_min, int16_t angleMax, uint16_t width_us_max);

	/**
	 * @brief Set servo position to the angle in degrees.
	 * @param angle degree of the final servo position to be set.
	 */
	void rotAbs(int16_t angle);

protected:
	int16_t curWidth;

private:
	int16_t angleMin, angleMax;
	uint16_t width_us_min, width_us_max;
	bool calibrateDone;
};

}

#endif
