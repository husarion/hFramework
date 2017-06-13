@NAME: sys_task
@CATEGORY: System
@DESC: This program will repeatedly print fizz buzz
#include <hFramework.h>

void buzzer()
{
	while (true)
	{
		printf("buzz\n");
		sys.delay(500);
	}
}

void hMain(void)
{
	sys.taskCreate(&buzzer);
	sys.delay(500);
	while (true)
	{
		printf("fizz\n");
		sys.delay(1000);
	}
}
