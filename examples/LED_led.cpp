@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: led
@CATEGORY: LED
#include <hFramework.h>

void hMain()
{
	// turn on LEDs
	hLED1.on();
	hLED2.on();
	hLED3.on();
	sys.delay(500);
	// turn off LEDs
	hLED1.off();
	hLED2.off();
	hLED3.off();

	while (true)
	{
		hLED1.toggle();  // switch state LED1
		sys.delay(100);
		hLED2.toggle();  // switch state LED2
		sys.delay(100); 
		hLED3.toggle();  // switch state LED3
		sys.delay(100);
	}
}
