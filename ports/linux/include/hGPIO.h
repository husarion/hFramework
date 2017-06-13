/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HGPIO_H__
#define __HGPIO_H__

#include <stdint.h>

#include <IGPIO.h>

namespace hFramework
{

/**
 * @brief Implementation of on-board General Purpose Input-Output pin (GPIO).
 */
class hGPIO
{
public:
	int nr;
	int id;

	hGPIO(int id): id(id) {}

	void init();

	void setOut();
	void setIn();

	bool read();

	void write(bool value);
};

extern hGPIO gpio3;
extern hGPIO gpio5;
extern hGPIO gpio7;
extern hGPIO gpio8;
extern hGPIO gpio10;
extern hGPIO gpio11;
extern hGPIO gpio12;
extern hGPIO gpio13;

}

#endif /* __HGPIO_H__ */
