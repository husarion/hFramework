#include <hFramework.h>

//connect hExt1 UART TX pin to hExt1 UART RX pin

void hMain()
{
	char received_data[20];
	char data_to_send[] = {"example"};
	sys.setSysLogDev(&devNull);								//turn off system logs on Serial
	sys.setLogDev(&Serial);									// setting USB-serial as a default printf output
	hExt1.serial.init(19200, Parity::None, StopBits::One);   // configure hExt serial with baudrate == 19200, none parity and with one stop bit
	for (;;)
	{
		hExt1.serial.write(data_to_send, sizeof(data_to_send));
		if (hExt1.serial.read(received_data, sizeof(data_to_send), 500))
		{
			hLED2.toggle();
			printf("hExt1.serial has received data: %s\r\n", received_data);
		}
		else
		{
			printf("no data received - check connections!\r\n");
		}
		sys.delay(500);
	}
}