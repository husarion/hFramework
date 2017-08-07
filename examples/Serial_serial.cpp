@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: serial
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		if (Serial.available() > 0) // checking Serial availability
		{
			char c = Serial.getch(); // getting one character from Serial
			switch (c)				 // deciding based on pulled charakter
			{
			case 'q': // in case of geting q
				Serial.printf("function 1\r\n"); // print "function 1" on USB-serial 
				hLED1.on();
				break;
			case 'a': // in case of geting a
				Serial.printf("function 2\r\n");
				hLED1.off();
				break;
			default: // in other case
				Serial.printf("you pulled unassigned char\r\n");
				hLED1.toggle();
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
		if (Stdio.available() > 0) // checking Serial availability
		{
			char c = Stdio.getch(); // getting one character from Serial
			switch (c)				// deciding based on pulled charakter
			{
			case 'q': // in case of geting q
				Stdio.printf("function 1\r\n");  // print "function 1"
				hLED1.on();
				break;
			case 'a': // in case of geting a
				Stdio.printf("function 2\r\n");
				hLED1.off();
				break;
			default: // in other case
				Stdio.printf("you pulled unassigned char\r\n");
				hLED1.toggle();
				break;
			}
		}
		sys.delay(10);
	}
}
