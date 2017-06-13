/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_LED_H
#define H_LED_H

#include "hTypes.h"
#include "hGPIO.h"

namespace hFramework
{

/**
 * @brief Controls the on-board LED
 */
class hLEDClass
{
	friend class hSystem;

public:
	hLEDClass(uint8_t nr);

	void on(); //!< Turn LED on.
	void off(); //!< Turn LED off.
	void toggle(); //!< Turn LED off, if it is on, or turn LED on, if it is off.
	void set(bool enabled);

private:
	void init();

	hLEDClass(const hLEDClass&) = delete;

	hGPIO gpio;
};

}

#endif /* H_LED_H */
