@PORTS: stm32
@BOARDS: core2
@NAME: spi
@CATEGORY: SPI
#include <hFramework.h>

void hMain()
{
	hExt.spi.setSpeed(SPISpeed::Speed42000);
	char cmd[] = {0xaa};
	hExt.spi.write(cmd, 1);
	for (;;)
	{
		char c[10];
		hExt.spi.read(c, sizeof(c));
		sys.delay(200);
	}
}
