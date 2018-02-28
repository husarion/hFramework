/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __II2C_H__
#define __II2C_H__

#include <stdint.h>

namespace hFramework
{

/**
 * @brief Interface for I2C.
 */
class II2C
{
public:

	/**
	 * @brief Set data rate.
	 * @param bps date rade in bits per second
	 */
	virtual void setDataRate(uint32_t bps) = 0;

	/**
	 * @brief Write data to I2C bus to device with specified address.
	 * @param addr device address
	 * @param data data to be written
	 * @param len number of bytes to write
	 * @return true, if operation succeed, false otherwise
	 */
	virtual bool write(uint8_t addr, uint8_t* data, uint32_t len) = 0;

	/**
	 * @brief Read data via I2C bus from device with specified address.
	 * @param addr device address
	 * @param data data to be read
	 * @param len number of bytes to read
	 * @return true, if operation succeed, false otherwise
	 */
	virtual bool read(uint8_t addr, uint8_t* data, uint32_t len) = 0;

	/**
	 * @brief Perform Write/Read operation on the I2C bus to device with specified address.
	 * @param addr device address
	 * @param dataTx data to be written
	 * @param lengthTx number of bytes to write
	 * @param dataRx data to be read
	 * @param lengthRx number of bytes to read
	 * @return true, if operation succeed, false otherwise
	 */
	virtual bool rw(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx, uint32_t rxDelay = 0) = 0;

	// deprecated
	bool read(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx);
};

}
#endif
