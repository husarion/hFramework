/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ISENSOR_H__
#define __ISENSOR_H__

#include <hTypes.h>
#include <II2C.h>
#include <IGPIO.h>
#include <ISerial.h>
#include <hSoftwareI2C.h>

namespace hFramework
{

class ISensor_i2c;

/**
 * @brief Interface to hSensor port.
 */
class ISensor
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
	 * @brief Return ADC and interrupt capable pin1 of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO_super& getPin1() = 0;

	/**
	 * @brief Return pin2 of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO& getPin2() = 0;

	/**
	 * @brief Return pin3 of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO& getPin3() = 0;

	/**
	 * @brief Return pin4 of hSensor port.
	 * @return pin object
	 */
	virtual IGPIO& getPin4() = 0;

	/**
	 * @brief Return proxy object for software I2C based hSensor object.
	 * @return i2c proxy hSensor object
	 */
	virtual ISensor_i2c& getSoftwareI2C() = 0;
};

/**
 * @brief Interface to I2C capable hSensor port.
 */
class ISensor_i2c : public virtual ISensor
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
class ISensor_serial : public virtual ISensor
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

/**
 * @brief Proxy object for accessing software I2C implementation on any hSensor port.
 */
class hSensor_softi2c : public virtual ISensor_i2c
{
public:
	hSensor_softi2c(ISensor& sens) : baseSens(sens), softI2C(sens) { }
	
	II2C& getI2C() override { return softI2C; }
	IGPIO_super& getPin1() override { return baseSens.getPin1(); }
	IGPIO& getPin2() override { return baseSens.getPin2(); }
	IGPIO& getPin3() override { return baseSens.getPin3(); }
	IGPIO& getPin4() override { return baseSens.getPin4(); }

	void selectI2C() override { baseSens.selectSoftI2C(); }
	void selectGPIO() override { baseSens.selectGPIO(); }
	void selectSoftI2C() override { baseSens.selectSoftI2C(); }

	hSensor_softi2c& getSoftwareI2C() override { return *this; }

	void init() { softI2C.init(); }
	
private:
	ISensor& baseSens;
	hSoftwareI2C softI2C;
	
	hSensor_softi2c(const hSensor_softi2c&) = delete;
};

}

#endif
