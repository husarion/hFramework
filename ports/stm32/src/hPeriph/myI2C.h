/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYI2C_H_
#define MYI2C_H_

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	I2C_ERROR_NONE,
	I2C_ERROR_NACK,
	I2C_ERROR_BUS_ERROR,
	I2C_ERROR_ARBITATION_LOST,
	I2C_ERROR_TIMEOUT,
} I2C_ERROR;

enum
{
#if BOARD(ROBOCORE) || BOARD(CORE2)
	MYI2C_EXT,
	MYI2C_SENS1,
	MYI2C_SENS2,
#elif BOARD(CORE2MINI)
	MYI2C_SENS1,
#else
#  error no board
#endif
};

void myI2C_init(uint8_t i2c_nr, uint32_t baudrate);
void myI2C_deinit(uint8_t i2c_nr);
I2C_ERROR myI2C_changeBaudrate(uint8_t i2c_nr, uint32_t baudrate);
I2C_ERROR myI2C_write(uint8_t i2c_nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize);
I2C_ERROR myI2C_read(uint8_t i2c_nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize);
I2C_ERROR myI2C_read_write(uint8_t i2c_nr, uint8_t addr, uint8_t* dataBuf_TX,
                           uint32_t bufSize_TX, uint8_t* dataBuf_RX, uint32_t bufSize_RX);
void myI2C_acquireGPIO(uint8_t i2c_nr);
void myI2C_releaseGPIO(uint8_t i2c_nr);

#ifdef __cplusplus
}
#endif

#endif /* MYI2C_H_ */
