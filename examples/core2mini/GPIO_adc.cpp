#include <hFramework.h>

void hMain()
{
	hSens1.pin1.enableADC();  // enable ADC on pin1 on hSens1 port 
	for(;;)
	{
		float val1 = hSens1.pin1.analogReadVoltage();  // read analog values (voltage  in [V])
		int val2 = hSens1.pin1.analogReadRaw(); // read analog value (in range from 0 to 4095)
		Serial.printf("Voltage hSens1.pin1: %f\tRaw data hExt.pin1: %d\r\n", val1, val2); // send string via USB Serial at 460800
		sys.delay(100);
	}
}
