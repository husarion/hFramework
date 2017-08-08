/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <hSystem.h>
#include <hSerial.h>
#include <hStorage.h>
#include <hUtils.h>

#define DEBUG_CATEGORY HSYSTEM
#include <debug.h>

#include <mutex>
#include <thread>
#include <sys/time.h>
#include <unistd.h>

namespace hFramework
{

uint64_t hSystem::hSystem::startTime = 0;

hSystem::hSystem()
{
	startTime = 0; // important!
	startTime = getRefTime();
}

hTask& hSystem::taskCreate(const std::function<void()>& handler, uint8_t priority, uint32_t stack, const char* desc)
{
	hTask* task = new hTask();
	task->thread = std::thread([task, handler]() {
		handler();
		task->running = false;
	});
	return *task;
}

void hTask::join() {
	thread.join();
}

void hSystem::delay(uint32_t delay)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
void hSystem::delayUs(uint32_t delay)
{
	std::this_thread::sleep_for(std::chrono::microseconds(delay));
}

unsigned long hSystem::getRefTime()
{
	struct timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);

	uint64_t curTime = uint64_t(tv.tv_sec) * 1000 + uint64_t(tv.tv_nsec) / 1000 / 1000;
	return (unsigned long)(curTime - startTime);
}

uint32_t hSystem::getRandNr()
{
	uint32_t d;
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(&d, 1, sizeof(d), fp);
	fclose(fp);
	return d;
}

uint64_t hSystem::getSerialNum()
{
	return 0;
}

void* hSystem::malloc(uint32_t size)
{
	return ::malloc(size);
}

void hSystem::free(void * ptr)
{
	::free(ptr);
}

hSystem sys;

}
