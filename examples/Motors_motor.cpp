@PORTS : stm32
@BOARDS : robocore,core2,core2mini
@NAME : motor
@CATEGORY : Motors
#include <hFramework.h>

void encoder()
{
	while (true)
	{
		Serial.printf("pos: %d\r\n", hMot1.getEncoderCnt()); // Print on Serial currently counted encoder (of Motor1) ticks
		hLED1.toggle();
		sys.delay(100);
	}
}

void hMain()
{
	sys.taskCreate(encoder); // This creates a task that will execute `encoder` concurrently
	while (true)
	{
		hMot1.setPower(500); // Set motor power to 500 (50% of max power)
		sys.delay(1000);
		hMot1.setPower(-500); // Set motor power to 500 (50% of max power) in reverse direction
		sys.delay(1000);
	}
}
