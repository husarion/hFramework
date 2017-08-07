@PORTS: stm32
@BOARDS: core2
@NAME: gpio_inout
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt.pin1.setIn_pd();  // setting pin1 on hExt as the input with pull down resistor
	hExt.pin2.setOut(); // setting pin2 on hExt as the output 
	while (true)
	{
		bool value = hExt.pin1.read(); // read from pin1 on hExt
		hExt.pin2.write(value);  // write valu to pin2 on hExt
		sys.delay(300);
	}
}


void hMain2()
{
	hExt.pin1.setOutOD();  // setting pin1 on hExt as the output open drain mode 
	while (true)
	{
		hExt.pin1.toggle(); // changing pin1 on hExt state between high impedance and low state
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
	hSens1.pin1.setIn_pd();  // setting pin1 on hSens1 as the input with pull down resistor
	hSens1.pin2.setOut();  // setting pin2 on hSens1 as the output 
	while (true)
	{
		bool value = hSens1.pin1.read();  // read from pin1 on hSens1
		hSens1.pin2.write(value);  // write valu to pin2 on hSens1
		sys.delay(300);
	}
}


void hMain2()
{
	hSens1.pin1.setOutOD();  // setting pin1 on hSens1 as the output open drain mode 
	while (true)
	{
		hSens1.pin1.toggle();  // changing pin1 on hSens1 state between high impedance and low state
		sys.delay(500);
	}
}
