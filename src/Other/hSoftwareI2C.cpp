/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSoftwareI2C.h"

#include <hSystem.h>
#include <ISensor.h>

namespace hFramework
{

hSoftwareI2C::hSoftwareI2C(IGPIO& sda, IGPIO& scl)
	: pinSDA(sda), pinSCL(scl)
{
}
hSoftwareI2C::hSoftwareI2C(ISensor& sens)
	: pinSDA(sens.getPin2()), pinSCL(sens.getPin1())
{
}

void hSoftwareI2C::init(uint32_t bps)
{
	pinSCL.setOutOD();
	pinSDA.setOutOD();
	setDataRate(bps);
}
void hSoftwareI2C::deinit()
{
}
void hSoftwareI2C::setDataRate(uint32_t bps)
{
	int32_t a = 7016096; // calculated for 168MHz
	int32_t b = -24;
	delayVal = a / bps + b;
}

bool hSoftwareI2C::write(uint8_t addr, uint8_t* data, uint32_t len)
{
	sendStart();
	int ack = writeByte((addr << 1) | 0);
	if (!ack)
	{
		sendStop();
		return false;
	}
	for (uint32_t i = 0; i < len; i++)
	{
		int ack = writeByte(data[i]);
		if (!ack)
		{
			sendStop();
			return false;
		}
	}
	sendStop();
	return true;
}
bool hSoftwareI2C::read(uint8_t addr, uint8_t* data, uint32_t len)
{
	sendStart();
	writeByte((addr << 1) | 1);
	for (uint32_t i = 0; i < len; i++)
		data[i] = readByte(i != len - 1);
	sendStop();
	return true;
}
bool hSoftwareI2C::rw(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx)
{
	if (!write(addr, dataTx, lengthTx))
		return false;

	pinSCL.write(0);
	sys.delayUs(500);
	pinSCL.write(1);

	if (!read(addr, dataRx, lengthRx))
		return false;

	return true;
}

void hSoftwareI2C::delay()
{
	volatile int cnt = delayVal;
	while (--cnt)
		__asm volatile("nop");
}

uint8_t hSoftwareI2C::writeByte(uint8_t v)
{
	pinSCL.write(0);
	for (int i = 0; i < 8; i++)
	{
		if (v & 0x80)
			pinSDA.write(1);
		else
			pinSDA.write(0);
		v <<= 1;
		delay();
		pinSCL.write(1);
		delay();
		pinSCL.write(0);
	}

	// ack
	pinSDA.write(1);
	delay();
	pinSCL.write(1);
	delay();
	int r = !pinSDA.read();
	pinSCL.write(0);
	pinSDA.write(0);
	return r;
}
uint8_t hSoftwareI2C::readByte(int ack)
{
	uint8_t v = 0;
	pinSDA.write(1);
	for (int i = 0; i < 8; i++)
	{
		v <<= 1;
		pinSCL.write(0);
		delay();
		pinSCL.write(1);
		delay();
		if (pinSDA.read())
			v |= 0x01;
	}
	pinSCL.write(0);

	if (ack)
	{
		pinSDA.write(0);
		delay();
		pinSCL.write(1);
		delay();
		pinSCL.write(0);
	}
	else
	{
		pinSDA.write(1);
		delay();
		pinSCL.write(1);
		delay();
		pinSCL.write(0);
	}
	pinSDA.write(0);

	return v;
}
void hSoftwareI2C::sendStart()
{
	pinSCL.write(1);
	pinSDA.write(1);
	delay();
	pinSDA.write(0);
	delay();
	pinSCL.write(0);
}
void hSoftwareI2C::sendStop()
{
	delay();
	pinSCL.write(1);
	delay();
	pinSDA.write(1);
	delay();
}
}
