#include <hFramework.h>

void hMain()
{
	hSens3.selectSerial();	 // switch hSens3 to serial mode configure hExt serial with default settings 
	hSens3.serial.init(19200); // configure hSens3 serial with baudrate == 19200
}