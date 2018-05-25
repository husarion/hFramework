#include <hFramework.h>

void hMain()
{
	hSens1.pin1.setIn_pd();  // setting pin1 on hSens1 as the input with pull down resistor
	hSens1.pin2.setOut();  // setting pin2 on hSens1 as the output 
	while (true)
	{
		bool value = hSens1.pin1.read();  // read from pin1 on hSens1
		hSens1.pin2.write(value);  // write valu to pin2 on hSens1
		sys.delay(300);
	}
}


void hMain2()
{
	hSens1.pin1.setOutOD();  // setting pin1 on hSens1 as the output open drain mode 
	while (true)
	{
		hSens1.pin1.toggle();  // changing pin1 on hSens1 state between high impedance and low state
		sys.delay(500);
	}
}