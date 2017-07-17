@NAME: basic
#include <hFramework.h>

void hMain() // main task containing your code
{
	LED1.on(); // turn on build-in LED #1 on PCB
	
	for(;;) // common loop for contious run!
	{
		LED1.toggle(); // switch state of LED1
		sys.delay(500); // wait 500ms
	}
}
 
