#include <hFramework.h>

void hMain()
{
	hExt.pin1.setIn_pd();  // setting pin1 on hExt as the input with pull down resistor
	hExt.pin2.setOut(); // setting pin2 on hExt as the output 
	while (true)
	{
		bool value = hExt.pin1.read(); // read from pin1 on hExt
		hExt.pin2.write(value);  // write value to pin2 on hExt
		sys.delay(300);
	}
}


void hMain2()
{
	hExt.pin1.setOutOD();  // setting pin1 on hExt as the output open drain mode 
	while (true)
	{
		hExt.pin1.toggle(); // changing pin1 on hExt state between high impedance and low state
		sys.delay(500);
	}
}