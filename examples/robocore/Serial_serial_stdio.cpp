#include <hFramework.h>

void hMain()
{
	sys.setLogDev(&Serial); // setting USB-serial as a default printf output

	while (true)
	{
		char c = getchar(); // getting one character from Serial
		switch (c)			// decision based on pulled character
		{
		case 'q':					  // in case of getting q
			printf("function 1\r\n"); // print "function 1" on default output (USB-serial)
			hLED1.on();
			break;
		case 'a': // in case of getting a
			printf("function 2\r\n");
			hLED1.off();
			break;
		default: // in other case
			printf("you pulled unassigned char\r\n");
			hLED1.toggle();
			break;
		}
	}
}
