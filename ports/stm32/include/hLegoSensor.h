/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HLEGOSENSOR_H__
#define __HLEGOSENSOR_H__

#include <hTypes.h>

#include <ILegoSensor.h>
#include <hSensor.h>

#include <hI2C.h>
#include <hGPIO.h>
#include <hSerial.h>

namespace hFramework
{

/**
 * @brief Base implementation for on-board hLegoSensor port.
 */
class hLegoSensor : public virtual ILegoSensor
{
public:
	hLegoSensor();

	void currentSourceOn(bool state);

	/**
	 * @brief Return hLegoSensor ID
	 * @return hLegoSensor ID
	 */
	hSens_ID getID() const { return getBaseSensConst().getID(); }

	/**
	 * @brief Return current hLegoSensor mode.
	 * @return current mode
	 */
	hSensor::EMode getMode() const { return getBaseSensConst().getMode(); }

	hGPIO_super& getPinIntAdc() override { return getBaseSens().getPin1(); }
	hGPIO& getPin1() override { return getBaseSens().getPin3(); }
	hGPIO& getPin2() override { return getBaseSens().getPin4(); }

	hSensor_softi2c& getSoftwareI2C() override { return getBaseSens().getSoftwareI2C(); }

	void selectGPIO() override;
	void selectSoftI2C() override;

protected:
	virtual hSensor& getBaseSens() = 0;
	virtual const hSensor& getBaseSensConst() const = 0;

	void deselect();
};

/**
 * @brief Implementation of simple on-board hLegoSensor port.
 */
class hLegoSensor_simple : public hLegoSensor, public virtual ILegoSensor
{
public:
	hLegoSensor_simple(hSensor_simple& sens);

	hSensor_simple& getBaseSens() override { return baseSens; }

	/**
	 * @brief ADC and interrupt capable pin1 of hLegoSensor socket.
	 */
	hGPIO_super& pinIntAdc;

	/**
	 * @brief pin3 of hLegoSensor socket.
	 */
	hGPIO& pin1;

	/**
	 * @brief pin4 of hLegoSensor socket.
	 */
	hGPIO& pin2;

private:
	hSensor_simple& baseSens;

	const hSensor_simple& getBaseSensConst() const override { return baseSens; }

	hLegoSensor_simple(const hLegoSensor_simple&) = delete;
};

/**
 * @brief Implementation of on-board I2C capable hLegoSensor port.
 */
class hLegoSensor_i2c : public hLegoSensor, public virtual ILegoSensor_i2c
{
public:
	hLegoSensor_i2c(hSensor_i2c& sens);

	hSensor_i2c& getBaseSens() override { return baseSens; }

	hI2C& getI2C() override { return baseSens.getI2C(); }

	void selectI2C() override;

	/**
	 * @brief ADC and interrupt capable pin1 of hLegoSensor socket.
	 */
	hGPIO_super& pinIntAdc;

	/**
	 * @brief pin3 of hLegoSensor socket.
	 */
	hGPIO& pin1;

	/**
	 * @brief pin4 of hLegoSensor socket.
	 */
	hGPIO& pin2;

private:
	hSensor_i2c& baseSens;

	const hSensor_i2c& getBaseSensConst() const override { return baseSens; }

	hLegoSensor_i2c(const hLegoSensor_i2c&) = delete;
};

/**
 * @brief Implementation of on-board Serial capable hLegoSensor port.
 */
class hLegoSensor_serial : public hLegoSensor, public virtual ILegoSensor_serial
{
public:
	hLegoSensor_serial(hSensor_serial& sens);

	hSensor_serial& getBaseSens() override { return baseSens; }

	hSerial& getSerial() override { return baseSens.getSerial(); }

	void selectSerial() override;

	/**
	 * @brief ADC and interrupt capable pin1 of hLegoSensor socket.
	 */
	hGPIO_super& pinIntAdc;

	/**
	 * @brief pin3 of hLegoSensor socket.
	 */
	hGPIO& pin1;

	/**
	 * @brief pin4 of hLegoSensor socket.
	 */
	hGPIO& pin2;

private:
	hSensor_serial& baseSens;

	const hSensor_serial& getBaseSensConst() const override { return baseSens; }

	hLegoSensor_serial(const hLegoSensor_serial&) = delete;
};

}

#endif
