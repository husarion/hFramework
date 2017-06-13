/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hLED.h"
#include "myGPIO.h"
#include "hUtils.h"
#include "hSystem.h"

namespace hFramework
{

const uint8_t hLEDDefs[] = { MYGPIO_LED_1, MYGPIO_LED_2, MYGPIO_LED_3 };

hLEDClass::hLEDClass(uint8_t nr) : gpio(hLEDDefs[nr])
{
}

void hLEDClass::init()
{
	gpio.init();
	gpio.setOut();
}

void hLEDClass::on()
{
	gpio.write(1);
}
void hLEDClass::off()
{
	gpio.write(0);
}
void hLEDClass::toggle()
{
	gpio.toggle();
}
void hLEDClass::set(bool enabled)
{
	gpio.write(enabled);
}

}
