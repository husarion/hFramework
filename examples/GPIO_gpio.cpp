@PORTS: stm32
@BOARDS: core2
@NAME: gpio
@DESC: Basic example of using GPIO
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt.pin1.setOut(); // setting hExt.pin1 as the output
	hSens1.pin1.setOut();  // setting hSens.pin1 as the output
	while (true)
	{
		hSens1.pin1.toggle(); // changing hSens.pin1 GPIO to opposite voltage level 
		hExt.pin1.toggle(); // changing hSens.pin1 GPIO to opposite voltage level 
		sys.delay(300);
	}
}
#####
@PORTS: stm32
@BOARDS: core2mini
@NAME: gpio
@DESC: Basic example of using GPIO
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hSens1.pin1.setOut();  // setting hSens.pin1 as the output 
	while (true)
	{
		hSens1.pin1.toggle(); // changing hSens.pin1 GPIO to opposite voltage level 
		sys.delay(300);
	}
}

@PORTS: stm32
@BOARDS: robocore
@NAME: gpio
@DESC: Basic example of using GPIO
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt1.pin1.setOut(); // setting hExt.pin1 as the output
	hSens1.pin1.setOut();  // setting hSens.pin1 as the output 
	while (true)
	{
		hExt1.pin1.toggle(); // changing hExt1.pin1 GPIO to opposite voltage level 
		hSens1.pin1.toggle(); // changing hSens.pin1 GPIO to opposite voltage level
		sys.delay(300);
	}
}
