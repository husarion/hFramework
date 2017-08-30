@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: buttons_threads
@DESC: Example of using button-related events in thread
@CATEGORY: Interfaces
#include <hFramework.h>

void buttons_thread_loop()
{
	while (true)
	{
		hBtn1.waitForPressed();  // waiting for press hBtn1
		printf("PRESSED\r\n");  
		hMot1.setPower(500);	 
		hBtn1.waitForReleased(); // waiting for released hBtn1
		printf("RELEASED\r\n");  
		hMot1.setPower(0); 	
	}
}

void hMain()
{
	sys.taskCreate(buttons_thread_loop); // create thread handling buttons, so you can do other work in hMain

	while (true) 
	{
		hLED1.toggle();  
		sys.delay(500); 
	}
}
