@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: buttons_threads
@CATEGORY: Interfaces
#include <hFramework.h>

void buttons_thread_loop()
{
	while (true)
	{
		hBtn1.waitForPressed();  // Waiting for press hBtn1
		printf("PRESSED\r\n");  
		hMot1.setPower(500);	 
		hBtn1.waitForReleased(); // Waiting for released hBtn1
		printf("RELEASED\r\n");  
		hMot1.setPower(0); 	
	}
}

void hMain()
{
	sys.taskCreate(buttons_thread_loop); // Create thread handling buttons, so you can do other work in hMain

	while (true) 
	{
		hLED1.toggle();  
		sys.delay(500); 
	}
}
