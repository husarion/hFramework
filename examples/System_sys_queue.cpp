@NAME: sys_queue
@CATEGORY: System
#include <hFramework.h>

// Create queue with capacity for 60 elements
hQueue<unsigned int> queue(60);

void consumer()
{
	while (true)
	{
		unsigned int number;
		// Pop element from the from of the queue. Will block if the queue is empty.
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
		// Push element to the queue. Will block if the queue is full.
		queue.sendToBack(i);
		i++;
	}
}
