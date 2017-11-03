#include "hFramework.h"
//#include "hCloudClient.h"
#include <stddef.h>
#include <stdio.h>

using namespace hFramework;

void hMain()
{
	sys.setLogDev(&Serial);
	//platform.begin(&RPi);
	for (;;)
	{
		hLED1.toggle();
		printf("test 111 %d\r\n", (int) sys.getRefTime());
		sys.delay(500);
	}
}
