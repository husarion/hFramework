#include <hFramework.h>

void hMain()
{
	// enable ADC on our ports
	hSens4.pinIntAdc.enableADC(); // enable ADC on pin1 on hSens4 port 
	hExt1.pin7.enableADC();     // enable ADC on pin7 on hExt1 port
	for(;;)
	{
		// read analog values (voltage)
		float val1 = hSens4.pinIntAdc.analogReadVoltage(); // read analog value (voltage in [V])
		float val2 = hExt1.pin7.analogReadRaw(); // read analog value (in range from 0 to 4095)
		Serial.printf("Voltage hSens4.pin1: %f\tRaw data hExt1.pin7: %d\r\n", val1, val2); // send string via USB Serial at 460800
		sys.delay(100);
	}
}