@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: motor
@CATEGORY: Motors
#include <hFramework.h>

void encoder()
{
	while (true)
	{
		// Print the current rotation of hMot1 in encoder ticks
		printf("pos: %d\r\n", hMot1.getEncoderCnt());
		LED1.toggle();
		sys.delay(100);
	}
}

void hMain()
{
	// This creates a task that will execute `encoder` concurrently
	sys.taskCreate(encoder);
	while (true)
	{
		// Set motor power to 500
		hMot1.setPower(500);
		// Sleep for 1 second
		sys.delay(1000);
		// Set motor power to 500 (in reverse direction)
		hMot1.setPower(-500);
		// Sleep for 1 second
		sys.delay(1000);
	}
}
