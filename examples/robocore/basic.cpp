#include <hFramework.h>

void hMain() // main task containing your code
{
	hLED1.on(); // turn on build-in LED1 on PCB
	
	for(;;) // common loop for continuous run
	{
		hLED1.toggle(); // switch state of hLED1
		sys.delay(500); // wait 500ms
	}
}
