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
