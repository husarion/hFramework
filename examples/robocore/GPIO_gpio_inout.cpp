#include <hFramework.h>

void hMain()
{
	hExt1.pin1.setIn_pd();  // setting pin1 on hExt1 as the input with pull down resistor
	hExt1.pin2.setOut(); // setting pin2 on hExt1 as the output 
	while (true)
	{
		bool value = hExt1.pin1.read(); // read from pin1 on hExt1
		hExt1.pin2.write(value);  // write value to pin2 on hExt1
		sys.delay(300);
	}
}


void hMain2()
{
	hExt1.pin1.setOutOD();  // setting pin1 on hExt1 as the output open drain mode 
	while (true)
	{
		hExt1.pin1.toggle(); // changing pin1 on hExt1 state between high impedance and low state
		sys.delay(500);
	}
}