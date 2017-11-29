/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HEXT_H__
#define __HEXT_H__

#include <hTypes.h>
#include <hGPIO.h>
#include <hSerial.h>
#include <hI2C.h>
#include <hSPI.h>
#include <hMutex.h>

namespace hFramework
{

#if BOARD(ROBOCORE)

typedef enum
{
	hExt_ID_1,
	hExt_ID_2,
} hExt_ID;

/**
 * @brief Interface for expansion header connector called hExt.
 */
class hExtClass
{
	hExt_ID id;

public:
	hExtClass(hExt_ID id);

	void init();

	/**
	 * @brief Pin 1 of the hExt socket. Interrupt capable.
	 */
	hGPIO_int   pin1;

	/**
	 * @brief Pin 2 of the hExt socket. Interrupt capable.
	 */
	hGPIO_int   pin2;

	/**
	 * @brief Pin 3 of the hExt socket.
	 */
	hGPIO       pin3;

	/**
	 * @brief Pin 4 of the hExt socket.
	 */
	hGPIO       pin4;

	/**
	 * @brief Pin 5 of the hExt socket.
	 */
	hGPIO       pin5;

	/**
	 * @brief Pin 6 of the hExt socket.
	 */
	hGPIO       pin6;

	/**
	 * @brief Pin 7 of the hExt socket. Interrupt and ADC capable.
	 */
	hGPIO_super pin7;

	hGPIO& getPin(int num);

	/**
	 * @brief Serial (UART) interface.
	 */
	hSerial serial;

	/**
	 * @brief SPI interface.
	 */
	hSPI& spi;

	/**
	 * @brief I2C interface.
	 */
	hI2C& i2c;

private:
	static void hExtInit();

	hExtClass(const hExtClass&) = delete;

	friend class hSystem;
};

#elif BOARD(CORE2)

/**
 * @brief Interface for expansion header connector called hExt.
 */
class hExtClass
{
public:
	hExtClass();

	void init();

	/**
	 * @brief Pin 1 of the hExt socket. Interrupt capable.
	 */
	hGPIO_super pin1;

	/**
	 * @brief Pin 2 of the hExt socket. Interrupt capable.
	 */
	hGPIO_super pin2;

	/**
	 * @brief Pin 3 of the hExt socket.
	 */
	hGPIO_adc   pin3;

	/**
	 * @brief Pin 4 of the hExt socket.
	 */
	hGPIO_adc   pin4;

	/**
	 * @brief Pin 5 of the hExt socket.
	 */
	hGPIO_adc   pin5;

	hGPIO& getPin(int num);

	/**
	 * @brief Serial (UART) interface.
	 */
	hSerial serial;

	/**
	 * @brief SPI interface.
	 */
	hSPI spi;

	/**
	 * @brief I2C interface.
	 */
	hI2C i2c;

private:
	static void hExtInit();

	hExtClass(const hExtClass&) = delete;

	friend class hSystem;
};

#endif

}

#endif /* __HEXT_H__ */
