/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSENSOR_H__
#define __HSENSOR_H__

#include <ISensor.h>

#include <hI2C.h>
#include <hGPIO.h>
#include <hSerial.h>

namespace hFramework
{

/**
 * @brief hSensor ID
 */
typedef enum
{
#if BOARD(ROBOCORE)
	hSens_ID_1, //!< hSensor1
	hSens_ID_2, //!< hSensor2
	hSens_ID_3, //!< hSensor3
	hSens_ID_4, //!< hSensor4
	hSens_ID_5, //!< hSensor5
#elif BOARD(CORE2)
	hSens_ID_1, //!< hSensor1
	hSens_ID_2, //!< hSensor2
	hSens_ID_3, //!< hSensor3
	hSens_ID_4, //!< hSensor4
	hSens_ID_5, //!< hSensor5
	hSens_ID_6, //!< hSensor6
#elif BOARD(CORE2MINI)
	hSens_ID_1, //!< hSensor1
	hSens_ID_2, //!< hSensor2
	hSens_ID_3, //!< hSensor3
#else
#  error no board
#endif
} hSens_ID;

class hLegoSensor;

/**
 * @brief Base implementation for on-board hSensor port.
 */
class hSensor : public virtual ISensor
{
	friend hLegoSensor;

public:
	enum EMode { MODE_NONE, MODE_GPIO, MODE_I2C, MODE_SERIAL, MODE_SOFTI2C };

	hSensor(hSens_ID id);

	/**
	 * @brief Initialize hSensor object
	 */
	virtual void init();

	/**
	 * @brief Return hSensor ID
	 * @return hSensor ID
	 */
	hSens_ID getID() const;

	/**
	 * @brief Return current hSensor mode.
	 * @return current mode
	 */
	EMode getMode() const { return mode; }

	hGPIO_super& getPin1() override { return pin1; }
	hGPIO& getPin2() override { return pin2; }
	hGPIO& getPin3() override { return pin3; }
	hGPIO& getPin4() override { return pin4; }

	hSensor_softi2c& getSoftwareI2C() override { return softI2C; }

	void selectGPIO() override;
	void selectSoftI2C() override;

	/**
	 * @brief ADC and interrupt capable pin1 of hSensor socket.
	 */
	hGPIO_super pin1;

	/**
	 * @brief pin2 of hSensor socket.
	 */
	hGPIO pin2;

	/**
	 * @brief pin3 of hSensor socket.
	 */
	hGPIO pin3;

	/**
	 * @brief pin4 of hSensor socket.
	 */
	hGPIO pin4;

protected:
	hSens_ID id;
	EMode mode;
	hSensor_softi2c softI2C;

	virtual void deselect();
};

/**
 * @brief Implementation of simple on-board hSensor port.
 */
class hSensor_simple : public hSensor, public virtual ISensor
{
public:
	hSensor_simple(hSens_ID id);

private:
	hSensor_simple(const hSensor_simple&) = delete;
};

/**
 * @brief Implementation of on-board I2C capable hSensor port.
 */
class hSensor_i2c : public hSensor, public virtual ISensor_i2c
{
public:
	hI2C i2c; //!< I2C interface.

	hSensor_i2c(hSens_ID id);

	hI2C& getI2C() override { return i2c; }

	void selectI2C() override;

private:
	hSensor_i2c(const hSensor_i2c&) = delete;

	void deselect() override;
};

/**
 * @brief Implementation of on-board Serial capable hSensor port.
 */
class hSensor_serial : public hSensor, public virtual ISensor_serial
{
public:
	hSerial serial; //! Serial interface.

	hSensor_serial(hSens_ID id);

	hSerial& getSerial() override { return serial; }

	void selectSerial() override;

private:
	hSensor_serial(const hSensor_serial&) = delete;
};

}

#endif
