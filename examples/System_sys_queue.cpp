@PORTS: stm32
@BOARDS: core2,core2mini
@NAME: sys_queue
@CATEGORY: System
#include <hFramework.h>

hQueue<unsigned int> queue(60); // Create queue with capacity for 60 elements

void consumer()
{
	while (true)
	{
		unsigned int number; // Pop element from of the queue. Will block if the queue is empty.
		queue.receive(number);
		printf("consumed %d\n", number);
	}
}

void hMain()
{
	sys.taskCreate(&consumer);
	unsigned int i = 0;
	while (true)
	{
		queue.sendToBack(i); // Push element to the queue. Will block if the queue is full.
		i++;
	}
}
