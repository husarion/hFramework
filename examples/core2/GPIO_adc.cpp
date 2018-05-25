#include <hFramework.h>

void hMain()
{
	hSens4.pin1.enableADC(); // enable ADC on pin1 on hSens4 port 
	hExt.pin1.enableADC(); // enable ADC on pin1 on hExt port
	for(;;)
	{
		float val1 = hSens4.pin1.analogReadVoltage(); // read analog value (voltage in [V])
		int val2 = hExt.pin1.analogReadRaw(); // read analog value (in range from 0 to 4095)
		Serial.printf("Voltage hSens4.pin1: %f\tRaw data hExt.pin1: %d\r\n", val1, val2); // send string via USB Serial at 460800
		sys.delay(100); 
	}
}