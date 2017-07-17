@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: serial
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		if (Serial.available() > 0)
		{
			char c = Serial.getch();
			switch (c)
			{
			case 'q':
				Serial.printf("function 1\r\n");
				break;
			case 'a':
				Serial.printf("function 2\r\n");
				break;
			default:
				Serial.printf("you pressed '%c'\r\n", c);
				break;
			}
		}
		sys.delay(10);
	}
}
#####
@PORTS: linux
@NAME: serial
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		if (Stdio.available() > 0)
		{
			char c = Stdio.getch();
			switch (c)
			{
			case 'q':
				Stdio.printf("function 1\r\n");
				break;
			case 'a':
				Stdio.printf("function 2\r\n");
				break;
			default:
				Stdio.printf("you pressed '%c'\r\n", c);
				break;
			}
		}
		sys.delay(10);
	}
}
