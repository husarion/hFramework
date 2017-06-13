@PORTS: stm32
@BOARDS: robocore
@NAME: gpio_inout
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// read from pin1 with pull down resistor
	hExt1.pin1.setIn_pd();
	hExt1.pin2.setOut();
	while (true)
	{
		// replicate value on pin1 to pin2
		bool value = hExt1.pin1.read();
		hExt1.pin2.write(value);
		sys.delay(300);
	}
}


void hMain2()
{
	hExt1.pin1.setOutOD();
	while (true)
	{
		// toggle between high impedance and low state
		hExt1.pin1.toggle();
		sys.delay(500);
	}
}
#####
@PORTS: stm32
@BOARDS: core2
@NAME: gpio_inout
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// read from pin1 with pull down resistor
	hExt.pin1.setIn_pd();
	hExt.pin2.setOut();
	while (true)
	{
		// replicate value on pin1 to pin2
		bool value = hExt.pin1.read();
		hExt.pin2.write(value);
		sys.delay(300);
	}
}


void hMain2()
{
	hExt.pin1.setOutOD();
	while (true)
	{
		// toggle between high impedance and low state
		hExt.pin1.toggle();
		sys.delay(500);
	}
}
#####
@PORTS: stm32
@BOARDS: core2mini
@NAME: gpio_inout
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// read from pin1 with pull down resistor
	hSens1.pin1.setIn_pd();
	hSens1.pin2.setOut();
	while (true)
	{
		// replicate value on pin1 to pin2
		bool value = hSens1.pin1.read();
		hSens1.pin2.write(value);
		sys.delay(300);
	}
}


void hMain2()
{
	hSens1.pin1.setOutOD();
	while (true)
	{
		// toggle between high impedance and low state
		hSens1.pin1.toggle();
		sys.delay(500);
	}
}
