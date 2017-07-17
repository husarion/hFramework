@PORTS: stm32
@BOARDS: robocore
@NAME: gpio
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt1.pin1.setOut();
	while (true)
	{
		hExt1.pin1.toggle();
		sys.delay(300);
	}
}
#####
@PORTS: stm32
@BOARDS: core2
@NAME: gpio
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt.pin1.setOut();
	while (true)
	{
		hExt.pin1.toggle();
		sys.delay(300);
	}
}
#####
@PORTS: stm32
@BOARDS: core2mini
@NAME: gpio
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hSens1.pin1.setOut();
	while (true)
	{
		hSens1.pin1.toggle();
		sys.delay(300);
	}
}
