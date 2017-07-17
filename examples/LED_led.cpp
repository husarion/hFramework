@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: led
@CATEGORY: LED
#include <hFramework.h>

void hMain()
{
	LED1.on();
	sys.delay(300);
	LED2.on();
	sys.delay(300);
	LED3.on();
	sys.delay(300);
	for (;;)
	{
		LED1.toggle();
		LED2.toggle();
		LED3.toggle();
		sys.delay(50);
	}
}
