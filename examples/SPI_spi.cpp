@PORTS: stm32
@BOARDS: core2
@NAME: spi
@CATEGORY: SPI
#include <hFramework.h>

void hMain()
{
	hExt.spi.setSpeed(SPISpeed::Speed42000); // configure hExt.spi with baudrate == 42000
	char cmd[] = {0xaa};					 // creating message
	hExt.spi.write(cmd, 1);					 // sending message thru SPI
	for (;;)
	{
		char c[10];					 // creating 10 element long char table
		hExt.spi.read(c, sizeof(c)); // geting 10 character from SPI
		sys.delay(200);
	}
}
