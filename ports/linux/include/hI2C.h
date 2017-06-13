/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HI2C_H__
#define __HI2C_H__

#include <stdint.h>

#include <hTypes.h>
#include <hGPIO.h>
#include <II2C.h>

namespace hFramework
{

enum hI2C_ID
{
	hI2C_ID_SENS1,
	hI2C_ID_SENS2,
	hI2C_ID_EXT,
	hI2C_ID_INVALID,
};

/**
 * @brief Implementation of on-board I2C interface.
 */
class hI2C : public II2C
{
	friend class hSensor_i2c;
	friend class hExt;

public:
	hGPIO pinScl, pinSda;

	hI2C(hI2C_ID no);

	void setDataRate(uint32_t bps);
	bool write(uint8_t addr, uint8_t* data, uint32_t len);
	bool read(uint8_t addr, uint8_t* data, uint32_t len);
	bool rw(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx);

	void selectGPIO();
	void selectI2C();

private:
	int i2cNum;

	void init(uint32_t bps = 10000);
	void deinit();

	hI2C(const hI2C&) = delete;
};

}

#endif
