#include <hFramework.h>`

void hMain()
{
	
	sys.setSysLogDev(&devNull);								//turn off system logs on Serial
	sys.setLogDev(&Serial);									// setting USB-serial as a default printf output
	Serial.init(19200, Parity::None, StopBits::One);		// configure USB-serial with baudrate == 19200, none parity and with one stop bit

	for (;;)
	{	
		uint32_t time = sys.getRefTime();
		printf("Current lifetime: %d\r\n",time);
		sys.delay(500);
	}
}