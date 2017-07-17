@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: buttons_simple
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		bool state = hBtn1.isPressed();
		printf("%d\r\n", state);

		// Is the button pressed?
		if (state)
		{
			// if it is, LEDs will be lit
			LED1.on();
			LED3.on();
		}
		else
		{
			// if it is, LEDs will be turned off
			LED1.off();
			LED3.off();
		}
		// wait 50 ms
		sys.delay(50);
	}
}
