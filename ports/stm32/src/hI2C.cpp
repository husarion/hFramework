/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hI2C.h"
#include "hGPIO.h"
#include "hSensor.h"
#include "myI2C.h"
#include "myUSART.h"
#include "hSystem.h"
#include "hUtils.h"
#include "debug.h"
#include "myGPIO.h"

namespace hFramework
{

struct
{
	int implNo;
	uint8_t pinScl, pinSda;
} const hI2CDefs[] =
{
#if BOARD(ROBOCORE)
	{ MYI2C_SENS1, MYGPIO_SENS1_DIGIA, MYGPIO_SENS1_DIGIB },
	{ MYI2C_SENS2, MYGPIO_SENS2_DIGIA, MYGPIO_SENS2_DIGIB },
	{ MYI2C_EXT,   MYGPIO_EXT_I2C_SCL, MYGPIO_EXT_I2C_SDA },
#elif BOARD(CORE2)
	{ MYI2C_SENS1, MYGPIO_SENS1_DIGIC, MYGPIO_SENS1_DIGID },
	{ MYI2C_SENS2, MYGPIO_SENS2_DIGIC, MYGPIO_SENS2_DIGID },
	{ MYI2C_EXT,   MYGPIO_EXT_I2C_SCL, MYGPIO_EXT_I2C_SDA },
#elif BOARD(CORE2MINI)
	{ MYI2C_SENS1, MYGPIO_SENS1_DIGIC, MYGPIO_SENS1_DIGID },
#else
#  error no board
#endif
};

static void printError(I2C_ERROR error)
{
	switch (error)
	{
	case I2C_ERROR_NACK: SYSLOG("I2C_NACK"); break;
	case I2C_ERROR_BUS_ERROR: SYSLOG("I2C_BERR"); break;
	case I2C_ERROR_ARBITATION_LOST: SYSLOG("I2C_ARLO"); break;
	case I2C_ERROR_TIMEOUT: SYSLOG("I2C_TIMEOUT"); break;
	case I2C_ERROR_NONE: return;
	}
}

hI2C::hI2C(hI2C_ID no)
	: pinScl(hI2CDefs[no].pinScl), pinSda(hI2CDefs[no].pinSda), i2cNum(hI2CDefs[no].implNo)
{
}

void hI2C::init(uint32_t bps)
{
	myI2C_init(i2cNum, bps);
}
void hI2C::deinit()
{
	myI2C_deinit(i2cNum);
}
void hI2C::setDataRate(uint32_t bps)
{
	myI2C_changeBaudrate(i2cNum, bps);
}

bool hI2C::write(uint8_t addr, uint8_t* data, uint32_t len)
{
	I2C_ERROR res = myI2C_write(i2cNum, addr, data, len);
	printError(res);
	return res > 0 ? false : true;
}
bool hI2C::read(uint8_t addr, uint8_t* data, uint32_t len)
{
	I2C_ERROR res = myI2C_read(i2cNum, addr, data, len);
	printError(res);
	return res > 0 ? false : true;;
}
bool hI2C::rw(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx)
{
	I2C_ERROR res = myI2C_read_write(i2cNum, addr, dataTx, lengthTx, dataRx, lengthRx);
	printError(res);
	return res > 0 ? false : true;;
}

void hI2C::selectGPIO()
{
	myI2C_releaseGPIO(i2cNum);
	pinScl.init();
	pinSda.init();
}
void hI2C::selectI2C()
{
	myI2C_acquireGPIO(i2cNum);
}

}
