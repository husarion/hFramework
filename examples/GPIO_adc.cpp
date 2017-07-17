@PORTS: stm32
@BOARDS: robocore
@NAME: adc
@DESC: This example shows how to use A/D converter.
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// enable ADC on our ports
	hSens4.pinIntAdc.enableADC();
	hExt1.pin7.enableADC();
	while (true)
	{
		// read analog values (voltage)
		float v1 = hSens4.pinIntAdc.analogReadVoltage();
		float v2 = hExt1.pin7.analogReadVoltage();
		printf("%f %f\r\n", v1, v2);
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: core2
@NAME: adc
@DESC: This example shows how to use A/D converter.
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// enable ADC on our ports
	hSens4.pin1.enableADC();
	hExt.pin1.enableADC();
	while (true)
	{
		// read analog values (voltage)
		float v1 = hSens4.pin1.analogReadVoltage();
		float v2 = hExt.pin1.analogReadVoltage();
		printf("%f %f\r\n", v1, v2);
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: core2mini
@NAME: adc
@DESC: This example shows how to use A/D converter.
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// enable ADC on our ports
	hSens1.pin1.enableADC();
	while (true)
	{
		// read analog values (voltage)
		float v1 = hSens1.pin1.analogReadVoltage();
		printf("%f\r\n", v1);
		sys.delay(50);
	}
}
