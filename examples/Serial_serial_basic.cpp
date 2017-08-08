@PORTS: stm32
@BOARDS: core2,robocore
@NAME: serial_basic
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	
	sys.setSysLogDev(&devNull);								//turn off system logs on Serial
	sys.setLogDev(&Serial);									// setting USB-serial as a default printf output
	Serial.init(19200, Parity::None, StopBits::One);		// configure USB-serial with baudrate == 19200, none parity and with one stop bit

	for (;;)
	{
		printf("Current lifetime: %ld\r\n",sys.getRefTime());
		sys.delay(500);
	}
}