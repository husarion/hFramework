/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hLegoSensor.h>

namespace hFramework
{

// hLegoSensor
hLegoSensor::hLegoSensor()
{
}

void hLegoSensor::currentSourceOn(bool state)
{
	getBaseSens().pin2.setOut();
	getBaseSens().pin2.write(state ? 1 : 0);
}

void hLegoSensor::selectGPIO()
{
	getBaseSens().selectGPIO();
}

void hLegoSensor::selectSoftI2C()
{
	getBaseSens().selectSoftI2C();
}

void hLegoSensor::deselect()
{
	getBaseSens().deselect();
}

// hLegoSensor_simple
hLegoSensor_simple::hLegoSensor_simple(hSensor_simple& sens)
	: baseSens(sens), pinIntAdc(sens.pin1), pin1(sens.pin3), pin2(sens.pin4) // sens.pin2 is current source control
{
}

// hLegoSensor_i2c
hLegoSensor_i2c::hLegoSensor_i2c(hSensor_i2c& sens)
	: baseSens(sens), pinIntAdc(sens.pin1), pin1(sens.pin3), pin2(sens.pin4) // sens.pin2 is current source control
{
}
void hLegoSensor_i2c::selectI2C()
{
	baseSens.selectI2C();
}

// hLegoSensor_serial
hLegoSensor_serial::hLegoSensor_serial(hSensor_serial& sens)
	: baseSens(sens), pinIntAdc(sens.pin1), pin1(sens.pin3), pin2(sens.pin4) // sens.pin2 is current source control
{
}
void hLegoSensor_serial::selectSerial()
{
	baseSens.selectSerial();
}

}
