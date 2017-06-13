/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ILEGOSENSOR_H__
#define __ILEGOSENSOR_H__

#include <hTypes.h>
#include <II2C.h>
#include <IGPIO.h>
#include <ISerial.h>
#include <ISensor.h>

namespace hFramework
{

class ILegoSensor_i2c;

/**
 * @brief Interface to hSensor port.
 */
class ILegoSensor
{
public:
	/**
	 * @brief Change pin1 and pin2 functions to GPIO.
	 * pin1 and pin2 can be used as GPIO only after execution of selectGPIO().
	 */
	virtual void selectGPIO() = 0;

	/**
	 * @brief Change pin1 and pin2 functions to software implementation of I2C interface.
	 */
	virtual void selectSoftI2C() = 0;

	/**
	 * @brief Change state of current source.
	 * @param state if true, current source is enabled
	 */
	virtual void currentSourceOn(bool state) = 0;

	/**
	 * @brief Return ADC and interrupt capable pin of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO_super& getPinIntAdc() = 0;

	/**
	 * @brief Return pin1 (DIGIA) of hSensor port.
	 * @return pin object
	 */

	virtual IGPIO& getPin1() = 0;
	/**
	 * @brief Return pin2 (DIGIB) of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO& getPin2() = 0;

	/**
	 * @brief Return proxy object for software I2C based hSensor object.
	 * @return i2c proxy hSensor object
	 */
	virtual ISensor_i2c& getSoftwareI2C() = 0;
};

/**
 * @brief Interface to I2C capable hSensor port.
 */
class ILegoSensor_i2c : public virtual ILegoSensor
{
public:
	/**
	 * @brief Return I2C interface.
	 * @return i2c interface
	 */
	virtual II2C& getI2C() = 0;

	/**
	 * @brief Change port mode to I2C.
	 *
	 * i2c can be used only after execution of this method.
	 */
	virtual void selectI2C() = 0;
};

/**
 * @brief Interface to Serial capable hSensor port.
 */
class ILegoSensor_serial : public virtual ILegoSensor
{
public:
	/**
	 * @brief Return Serial interface.
	 * @return serial interface
	 */
	virtual ISerial& getSerial() = 0;

	/**
	 * @brief Change port mode to Serial.
	 *
	 * serial can be used only after execution of this method.
	 */
	virtual void selectSerial() = 0;
};

}
#endif
