@PORTS: stm32
@BOARDS: robocore
@NAME: serial_config
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	// configure hExt1 serial with baudrate == 9600, odd parity and with one stop bit
	hExt1.serial.init(9600, Parity::Odd, StopBits::One);
	// switch hSens3 to serial mode and configure it with baudrate == 19200 and default settings
	hSens3.selectSerial();
	hSens3.getSerial().init(19200);
}
#####
@PORTS: stm32
@BOARDS: core2
@NAME: serial_config
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	// configure hExt serial with baudrate == 9600, odd parity and with one stop bit
	hExt.serial.init(9600, Parity::Odd, StopBits::One);
	// switch hSens3 to serial mode and configure it with baudrate == 19200 and default settings
	hSens3.selectSerial();
	hSens3.serial.init(19200);
}
#####
@PORTS: stm32
@BOARDS: core2mini
@NAME: serial_config
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	// switch hSens3 to serial mode and configure it with baudrate == 19200 and default settings
	hSens3.selectSerial();
	hSens3.serial.init(19200);
}
