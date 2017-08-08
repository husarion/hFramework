@PORTS: stm32
@BOARDS: core2,core2mini,robocore
@NAME: sys_queue
@CATEGORY: System
#include <hFramework.h>

hQueue<unsigned int> queue(60); // create queue with capacity for 60 elements

void consumer()
{
	while (true)
	{
		unsigned int number; // pop element from of the queue. Will block if the queue is empty.
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
		queue.sendToBack(i); // push element to the queue. Will block if the queue is full.
		i++;
	}
}
