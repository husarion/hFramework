@PORTS: stm32
@BOARDS: core2
@NAME: power_supply
@CATEGORY: GPIO
/*
	The best way to reset devices conected to hSensor ports is to turn of power supply. Code below shows how to turn off supply line of hSens and h Ext port to reset problematic devices.
*/
#include <hFramework.h>

void resethSensorsPS(){
	sys.disable5V();	// turn off switching transistor at the hSensor 5V supply line
	sys.delay(200); 	// wait for discharging capacitors
	sys.enable5V();		// turn on switching transistor again
	sys.delay(50);		// wait for stable reading from sensors
}

void hMain()
{
	resethSensorsPS();	// reset 5V power supply at hSens and hExt ports
	
	for(;;){
		sys.delay(100);
		hLED1.toggle();
	}
}