/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdarg.h>

#include "debug.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define PERIPHERAL_BASE 0x40000000
#define BITBAND_PERIPHERAL_BASE 0x42000000
#define BITBAND_PERIPHERAL(addr,bit) (BITBAND_PERIPHERAL_BASE + ((uint32_t)(addr) - PERIPHERAL_BASE) * 32 + ((bit) * 4))

#ifdef __cplusplus
extern "C" {
#endif

void sys_idle_task();
void sys_tick_hook();

int sys_log(const char* format, ...);
int sys_syslog(const char* format, ...);
int sys_vlog(const char* format, va_list arg);
void sys_fault_log(const char* msg);
int sys_write_log(const char* buf, int len);
int sys_read_log(char* buf, int len);
int fail_log(const char* format, ...);

void sys_delay_ms_spin(uint32_t time);
void sys_delay_us_spin(uint32_t time);
void sys_delay_ms(uint32_t time);
void sys_delay_us(uint32_t time);

uint32_t msToTicks(uint32_t ms);
uint32_t usToTicks(uint32_t ms);
void sys_fault_handler();
#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#define constexpr static inline
#endif

constexpr uint32_t ticksPerMs()
{
	return configTICK_RATE_HZ / 1000;
}
constexpr uint32_t ticksPerUsMult(uint32_t mult)
{
	return configTICK_RATE_HZ * mult / 1000 / 1000;
}
constexpr uint32_t maxMsToWait()
{
	return portMAX_DELAY / ticksPerMs();
}
constexpr uint32_t maxUsToWait()
{
	return portMAX_DELAY / configTICK_RATE_HZ;
}
constexpr uint32_t tickDuraionInUs()
{
	return 1000 * 1000 / configTICK_RATE_HZ;
}


#ifndef __cplusplus
#undef constexpr
#endif

#endif
