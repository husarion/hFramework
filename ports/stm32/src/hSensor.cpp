/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hSensor.h>
#include <hSystem.h>

#include <myEXTI.h>
#include <myI2C.h>
#include <myGPIO.h>

#include <myUSART.h>
#include <hUtils.h>

#define DEBUG_CATEGORY HSENSOR
#include <debug.h>

namespace hFramework
{

struct
{
	hSerial_ID serialNo;
	hI2C_ID i2cNo;
	uint8_t pin1Nr, pin2Nr, pin3Nr, pin4Nr;
} const hSensorDefs[] =
{
#if BOARD(ROBOCORE)
	{ hSerial_ID_INVALID, hI2C_ID_SENS1,   MYGPIO_SENS1_INT_ADC, MYGPIO_SENS1_ON, MYGPIO_SENS1_DIGIA, MYGPIO_SENS1_DIGIB }, // hSens1
	{ hSerial_ID_INVALID, hI2C_ID_SENS2,   MYGPIO_SENS2_INT_ADC, MYGPIO_SENS2_ON, MYGPIO_SENS2_DIGIA, MYGPIO_SENS2_DIGIB }, // hSens2
	{ hSerial_ID_SENS3,   hI2C_ID_INVALID, MYGPIO_SENS3_INT_ADC, MYGPIO_SENS3_ON, MYGPIO_SENS3_DIGIA, MYGPIO_SENS3_DIGIB }, // hSens3
	{ hSerial_ID_INVALID, hI2C_ID_INVALID, MYGPIO_SENS4_INT_ADC, MYGPIO_SENS4_ON, MYGPIO_SENS4_DIGIA, MYGPIO_SENS4_DIGIB }, // hSens4
	{ hSerial_ID_INVALID, hI2C_ID_INVALID, MYGPIO_SENS5_INT_ADC, MYGPIO_SENS5_ON, MYGPIO_SENS5_DIGIA, MYGPIO_SENS5_DIGIB }, // hSens5
#elif BOARD(CORE2)
	{ hSerial_ID_INVALID, hI2C_ID_SENS1,   MYGPIO_SENS1_DIGIA_INT_ADC, MYGPIO_SENS1_DIGIB, MYGPIO_SENS1_DIGIC, MYGPIO_SENS1_DIGID }, // hSens1
	{ hSerial_ID_INVALID, hI2C_ID_SENS2,   MYGPIO_SENS2_DIGIA_INT_ADC, MYGPIO_SENS2_DIGIB, MYGPIO_SENS2_DIGIC, MYGPIO_SENS2_DIGID }, // hSens2
	{ hSerial_ID_SENS3,   hI2C_ID_INVALID, MYGPIO_SENS3_DIGIA_INT_ADC, MYGPIO_SENS3_DIGIB, MYGPIO_SENS3_DIGIC, MYGPIO_SENS3_DIGID }, // hSens3
	{ hSerial_ID_SENS4,   hI2C_ID_INVALID, MYGPIO_SENS4_DIGIA_INT_ADC, MYGPIO_SENS4_DIGIB, MYGPIO_SENS4_DIGIC, MYGPIO_SENS4_DIGID }, // hSens4
	{ hSerial_ID_INVALID, hI2C_ID_INVALID, MYGPIO_SENS5_DIGIA_INT_ADC, MYGPIO_SENS5_DIGIB, MYGPIO_SENS5_DIGIC, MYGPIO_SENS5_DIGID }, // hSens5
	{ hSerial_ID_INVALID, hI2C_ID_INVALID, MYGPIO_SENS6_DIGIA_INT_ADC, MYGPIO_SENS6_DIGIB, MYGPIO_SENS6_DIGIC, MYGPIO_SENS6_DIGID }, // hSens6
#elif BOARD(CORE2MINI)
	{ hSerial_ID_INVALID, hI2C_ID_SENS1,   MYGPIO_SENS1_DIGIA_INT_ADC, MYGPIO_SENS1_DIGIB, MYGPIO_SENS1_DIGIC, MYGPIO_SENS1_DIGID }, // hSens1
	{ hSerial_ID_SENS2,   hI2C_ID_INVALID, MYGPIO_SENS2_DIGIA_INT_ADC, MYGPIO_SENS2_DIGIB, MYGPIO_SENS2_DIGIC, MYGPIO_SENS2_DIGID }, // hSens2
	{ hSerial_ID_SENS3,   hI2C_ID_INVALID, MYGPIO_SENS3_DIGIA_INT_ADC, MYGPIO_SENS3_DIGIB, MYGPIO_SENS3_DIGIC, MYGPIO_SENS3_DIGID }, // hSens3
#else
#  error no board
#endif
};

// hSensor
hSensor::hSensor(hSens_ID id)
	: pin1(hSensorDefs[id].pin1Nr), pin2(hSensorDefs[id].pin2Nr), pin3(hSensorDefs[id].pin3Nr), pin4(hSensorDefs[id].pin4Nr),
	  id(id), mode(MODE_NONE), softI2C(*this)
{
}
void hSensor::init()
{
	SYSLOG("hSensor::init %d", id);
	pin1.init();
	pin2.init();
	pin3.init();
	pin4.init();
	selectGPIO();
}

hSens_ID hSensor::getID() const
{
	return id;
}

// void hSensor::currentSourceOn(bool state)
// {
// currentSrcPin.write(state ? 1 : 0);
// }

void hSensor::deselect()
{
	mode = MODE_NONE;
}

void hSensor::selectGPIO()
{
	deselect();
	if (mode == MODE_GPIO)
		return;

	pin3.init();
	pin4.init();
	mode = MODE_GPIO;
}

void hSensor::selectSoftI2C()
{
	deselect();
	if (mode == MODE_SOFTI2C)
		return;

	pin3.init();
	pin4.init();
	softI2C.init();
	mode = MODE_SOFTI2C;
}

// hSensor_simple
hSensor_simple::hSensor_simple(hSens_ID id) : hSensor(id)
{
}

// hSensor_i2c
hSensor_i2c::hSensor_i2c(hSens_ID id)
	: hSensor(id), i2c(hSensorDefs[id].i2cNo)
{
}
void hSensor_i2c::selectI2C()
{
	deselect();
	if (mode == MODE_I2C)
		return;

	i2c.init();
	mode = MODE_I2C;
}

void hSensor_i2c::deselect()
{
	if (mode == MODE_I2C)
		i2c.deinit();
	mode = MODE_NONE;
}

// hSensor_serial
hSensor_serial::hSensor_serial(hSens_ID id)
	: hSensor(id), serial(hSensorDefs[id].serialNo)
{
}
void hSensor_serial::selectSerial()
{
	deselect();
	if (mode == MODE_SERIAL)
		return;

	serial.init();
	mode = MODE_SERIAL;
}

}
