@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: tasks
@CATEGORY: System
#include <hFramework.h>
#include <cmath>

void task1()
{
	for (;;)
	{
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		// robAbs with true as 'block' parameter blocks execution until motor reaches desired position
		// angle, max power, block
		hMotA.rotAbs(pos * 90.0f, 1000, true);
	}
}

void task2()
{
	for (;;)
	{
		LED1.toggle();
		sys.delay(50);
	}
}

void hMain()
{
	sys.taskCreate(task1);
	sys.taskCreate(task2);
}
