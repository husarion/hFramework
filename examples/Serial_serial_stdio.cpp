@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: serial_stdio
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	sys.setLogDev(&Serial);

	while (true)
	{
		char c = getchar();
		switch (c)
		{
		case 'q':
			printf("function 1\r\n");
			break;
		case 'a':
			printf("function 2\r\n");
			break;
		default:
			printf("you pressed '%c'\r\n", c);
			break;
		}
	}
}
