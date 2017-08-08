@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: buttons_simple
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		bool state = hBtn1.isPressed();  // creating a variable containig value of hBtn1.isPressed
		printf("%d\r\n", state); 

		if (state)  // checking if the button is pressed. If it is, LEDs will be turned on
		{ 
			hLED1.on();
			hLED3.on();
		}
		else  // if it isn't, LEDs will be turned off
		{
			hLED1.off();
			hLED3.off();
		}
		sys.delay(50);
	}
}
