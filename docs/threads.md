# Tasks

--example c++ sys_task.cpp stripheaders:

hFramework uses FreeRTOS as a task scheduler and thus has first class support for multithreading/multitasking.

You can use [sys.taskCreate](%%hSystem::taskCreate%%) to create a new task. It will immediately begin executing.

--seeAlso hSystem

## Mutal exclusion

--example c++ sys_mutex.cpp stripheaders:

When writing multithread programs, the understanding [mutual exclusion](https://en.wikipedia.org/wiki/Mutual_exclusion) is important. When two tasks perform non-atomic modification to the global state they may interfere with each other in an unexpected ways.

hFramework provides mutexes -- only one task can hold a mutex at a single time -- when other process tries to take it, it will be blocked until the first one releases it.

--seeAlso hMutex hMutexGuard

## Queues

--example c++ sys_queue.cpp stripheaders:

hQueue is a thread safe multi-consumer multi-producer queue implementation.

* [receive](%%hQueue::receive%%) pops an element from the from of the queue. Blocks if queue is empty.
* [sendToBack](%%hQueue::sendToBack%%) pushes element to the back of the queue. Blocks if queue is full. Together with `receive` they form a FIFO queue.
{% if port(stm32) %}
* [sendToFront](%%hQueue::sendToFront%%) pushes element to the front of the queue. Blocks if queue is full. Together with `receive` they form a LIFO queue (a stack).
{% endif %}

--seeAlso hQueue
