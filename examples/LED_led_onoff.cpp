@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: led_onoff
@CATEGORY: LED
#include <hFramework.h>

void hMain()
{
	LED1.off(); // initially off
	LED2.on(); // LED2 will stay on
	while (true)
	{
		LED1.toggle(); // toggle LED1
		sys.delay(500); // wait for 500 ms
	}
}
