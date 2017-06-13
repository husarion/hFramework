#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include "hFramework.h"

hMotor hMot1(1);
int bytes = 0;

void led1()
{
	while (1)
	{
		sys.delay(1000);
		sys.log("b %d\r\n", bytes);
		bytes = 0;
	}
}
void led()
{
	int i = 0;
	while (1)
	{
		if (Usb.isConnected())
			LED1.on();
		else
			LED1.off();
		LED2.toggle();
		
		char d[50];
		if (Usb.isConnected())
		{
			if (Usb.isDataAvailable())
			{
				int r = Usb.read(d, 50, 1000);
				// sys.log("read %d\r\n", r);
				
				if (r > 0)
				{
				
					bytes += r;
					if (d[0] == 'S')
					{
						int val;
						sscanf(d + 1, "%d", &val);
						// sys.log("val %d\r\n", val);
						hMot1.setPower((val - 50) * 6);
					}
				}
			}
			
			char t[300];
			static int cnt = 0;
			sprintf(t, "S%d", cnt++);
			int r = Usb.write(t, 63, 500);
			if (r != 0)
				sys.log("send res: %d\r\n", r);
		}
		sys.delay(1);
	}
}

void hMain(void)
{
	sys.setLogDev(&Serial);
	sys.log("Start!\n\r");
	sys.taskCreate(led);
	sys.taskCreate(led1);
}
