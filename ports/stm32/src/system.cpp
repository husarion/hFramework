/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <system.h>
#include <hSystem.h>

using namespace hFramework;

void sys_fault_handler()
{
	sys.fault_handler();
}

void sys_tick_hook()
{
	sys.tickHook();
}

void sys_idle_task()
{
	sys.idleTask();
}

int sys_log(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return sys_vlog(format, arg);
}
int sys_vlog(const char* format, va_list arg)
{
	return hFramework::sys.vlog(format, arg);
}
int sys_syslog(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return hFramework::sys.vsyslog(format, arg);
}

namespace hFramework
{
extern hPrintfDev* logDev;
extern hPrintfDev* syslogDev;
}
int sys_write_log(const char* buf, int len)
{
	return hFramework::logDev->write(buf, len);
}
int sys_read_log(char* buf, int len)
{
	return hFramework::logDev->read(buf, len, INFINITE);
}

int fail_log(const char* str, ...)
{
	va_list arg;
	va_start(arg, str);
	return hFramework::sys.fail_vlog(str, arg);
}

void sys_delay_ms_spin(uint32_t time)
{
	sys_delay_us_spin(time * 1000);
}
void sys_delay_us_spin(uint32_t time)
{
	// adjusted by hand as us timing is not critical
#define CAL_FACTOR 18
	volatile int cnt = time * CAL_FACTOR;
	while (--cnt)
	{
		__asm volatile("nop");
	}
}
void sys_delay_ms(uint32_t time)
{
	hFramework::sys.delay(time);
}
void sys_delay_us(uint32_t time)
{
	hFramework::sys.delayUs(time);
}
