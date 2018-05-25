#include <hFramework.h>
#include <cmath>

void buzzer() // creating instruction for buzzer
{
	while (true)
	{
		Serial.printf("buzz\n");
		sys.delay(500);
	}
}

void motors() //creating instruction for motors
{
	for (;;)
	{
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		hMotA.rotAbs(pos * 90.0f, 300, true); // robAbs with true as 'block' parameter blocks execution until motor reaches desired position
	}
}

void tog() //creating instruction for tog
{
	for (;;)
	{
		hLED1.toggle();
		sys.delay(50);
	}
}

void hMain()
{
	sys.taskCreate(&buzzer); // running task buzzer
	sys.taskCreate(&motors); // running task motors
	sys.taskCreate(&tog);	//runing task tog
	while (true)
	{
		Serial.printf("fizz\n");
		sys.delay(1000);
	}
}
