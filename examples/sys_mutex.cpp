@NAME: sys_mutex
@CATEGORY: System
#include <hFramework.h>

int counter = 0;
hMutex counter_mutex;

void add_1()
{
	// Modify counter in mutex to ensure that the modification is not lost
	counter_mutex.take();
	int new_value = counter + 1;
	counter = new_value;
	counter_mutex.give();
}

void adder()
{
	while (true)
	{
		add_1();
		sys.delay(500);
	}
}

void hMain()
{
	sys.taskCreate(&adder);
	while (true)
	{
		add_1();
		printf("fizz\n");
		sys.delay(1000);
	}
}
