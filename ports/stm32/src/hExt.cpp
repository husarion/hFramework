/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hExt.h>
#include <hSystem.h>

#include <myUSART.h>
#include <hSerial.h>
#include <myGPIO.h>
#include <hSystem.h>
#include <hUtils.h>
#include <hI2C.h>

#include <FreeRTOS.h>
#include <task.h>

#define DEBUG_CATEGORY HEXT
#include <debug.h>

namespace hFramework
{

#if BOARD(ROBOCORE)

extern hI2C hExtI2C;
extern hSPI hExtSPI;

struct
{
	hSerial_ID serialNo;
	uint8_t pin1, pin2, pin3, pin4, pin5, pin6, pin7;
} const hExtDefs[] =
{
#if BOARD(ROBOCORE)
	{ hSerial_ID_EXT1, MYGPIO_EXT1_PIN1, MYGPIO_EXT1_PIN2, MYGPIO_EXT1_PIN3, MYGPIO_EXT1_PIN4, MYGPIO_EXT1_PIN5, MYGPIO_EXT1_PIN6, MYGPIO_EXT1_PIN7, },
	{ hSerial_ID_EXT2, MYGPIO_EXT2_PIN1, MYGPIO_EXT2_PIN2, MYGPIO_EXT2_PIN3, MYGPIO_EXT2_PIN4, MYGPIO_EXT2_PIN5, MYGPIO_EXT2_PIN6, MYGPIO_EXT2_PIN7, },
#endif
#if BOARD(CORE2)
#warning implement
#endif
};

void hExtClass::hExtInit()
{
	hExtI2C.init(10000);
	hExtSPI.init();
}

hExtClass::hExtClass(hExt_ID id)
	: id(id),
	  pin1(hExtDefs[id].pin1),
	  pin2(hExtDefs[id].pin2),
	  pin3(hExtDefs[id].pin3),
	  pin4(hExtDefs[id].pin4),
	  pin5(hExtDefs[id].pin5),
	  pin6(hExtDefs[id].pin6),
	  pin7(hExtDefs[id].pin7),
	  serial(hExtDefs[id].serialNo),
	  spi(hExtSPI),
	  i2c(hExtI2C)
{
}

void hExtClass::init()
{
	pin1.init();
	pin2.init();
	pin3.init();
	pin4.init();
	pin5.init();
	pin6.init();
	pin7.init();

	serial.init(230400, Parity::None, StopBits::One);

	SYSLOG("hExtClass::init %d", id);
}

hGPIO& hExtClass::getPin(int num)
{
	switch (num)
	{
	default:
	case 1: return pin1;
	case 2: return pin2;
	case 3: return pin3;
	case 4: return pin4;
	case 5: return pin5;
	case 6: return pin6;
	case 7: return pin7;
	case 8: return serial.pinRx;
	case 9: return serial.pinTx;
	}
}

#elif BOARD(CORE2)

void hExtClass::hExtInit()
{
}

hExtClass::hExtClass()
	: pin1(MYGPIO_EXT_PIN1),
	  pin2(MYGPIO_EXT_PIN2),
	  pin3(MYGPIO_EXT_PIN3),
	  pin4(MYGPIO_EXT_PIN4),
	  pin5(MYGPIO_EXT_PIN5),
	  serial(hSerial_ID_EXT),
	  spi(hSPI_ID_EXT),
	  i2c(hI2C_ID_EXT)
{
}

void hExtClass::init()
{
	spi.init();
	i2c.init();

	pin1.init();
	pin2.init();
	pin3.init();
	pin4.init();
	pin5.init();

	serial.init(230400, Parity::None, StopBits::One);

	SYSLOG("hExtClass::init");
}

hGPIO& hExtClass::getPin(int num)
{
	switch (num)
	{
	default:
	case 1: return pin1;
	case 2: return pin2;
	case 3: return pin3;
	case 4: return pin4;
	case 5: return pin5;
	case 6: return serial.pinRx;
	case 7: return serial.pinTx;
	case 8: return spi.pinSck;
	case 9: return spi.pinMiso;
	case 10: return spi.pinMosi;
	case 11: return i2c.pinSda;
	case 12: return i2c.pinScl;
	}
}

#endif

}
