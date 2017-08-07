@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: basic
#include <hFramework.h>

void hMain() // main function containing your code
{
	hLED1.on(); // turn on build-in LED1 on PCB
	
	for(;;) // common loop for contious run
	{
		hLED1.toggle(); // switch state of LED1
		sys.delay(500); // wait 500ms
	}
}
 
