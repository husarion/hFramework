@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: buttons_threads
@CATEGORY: Interfaces
#include <hFramework.h>

void buttons_thread_loop()
{
	while (true)
	{
		hBtn1.waitForPressed();
		printf("PRESSED\r\n");
		hMot1.setPower(500);
		hBtn1.waitForReleased();
		printf("RELEASED\r\n");
		hMot1.setPower(0);
	}
}

void hMain()
{
	// create thread handling buttons, so you can do other work in hMain
	sys.taskCreate(buttons_thread_loop);

	while (true)
	{
		LED1.toggle();
		sys.delay(500);
	}
}
