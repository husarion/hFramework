/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx.h"

#include <hSystem.h>
#include <hSerial.h>
#include <hUSB.h>
#include <hButton.h>
#include <hStorage.h>
#include <hServoModule.h>
#include <hMotorPimpl.h>
#include <hPrintf.h>
#include <hUtils.h>

#define DEBUG_CATEGORY HSYSTEM
#include <debug.h>

#include <myUSART.h>
#include <myGPIO.h>
#include <myADC.h>
#include <myTimer.h>

#include "port.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "system.h"

namespace hFramework
{

hSystem sys;
hPrintfDev* logDev = &DevNull;
hPrintfDev* syslogDev = &DevNull;

hGPIO_adc supplySensePin(MYGPIO_BAT_MEAS);
#if BOARD(ROBOCORE) || BOARD(CORE2)
hGPIO pinUsbCharging(MYGPIO_USB_CHARGE);
#endif
#if BOARD(CORE2) || BOARD(CORE2MINI)
hGPIO pin5V(MYGPIO_SENS_POWER_ON);
#endif

hStorage storage;

const TRoboCOREHeader* getRobocoreHeader();

void hSystem::init()
{
	hLED1.init();
	hLED2.init();
	hLED3.init();
	hLED1.off();
	hLED2.off();
	hLED3.off();

	myTimerInit();

	storage.init();

	supplySensePin.init();
	supplySensePin.enableADC();

#if BOARD(ROBOCORE) || BOARD(CORE2)
	pinUsbCharging.init();
	pinUsbCharging.setOut();
	enableUsbCharging();
#endif

#if BOARD(ROBOCORE)
	Usb.init();
#endif

#if BOARD(CORE2) || BOARD(CORE2MINI)
	pin5V.init();
	pin5V.setOut();
	pin5V.write(0);
#endif

	// Motors
	hMotorPimpl::setupDrivers();
#if BOARD(ROBOCORE)
	hMot1.init();
	hMot2.init();
	hMot3.init();
	hMot4.init();
	hMot5.init();
	hMot6.init();
#elif BOARD(CORE2)
	hMot1.init();
	hMot2.init();
	hMot3.init();
	hMot4.init();
#elif BOARD(CORE2MINI)
	hMot1.init();
	hMot2.init();
#else
#  error no board
#endif
	hMotorPimpl::setDefaults();

#if BOARD(ROBOCORE)
	hBaseSens1.init();
	hBaseSens2.init();
	hBaseSens3.init();
	hBaseSens4.init();
	hBaseSens5.init();

	hExtClass::hExtInit();
	hExt1.init();
	hExt2.init();

	hBtn1.init();
	hBtn2.init();
#elif BOARD(CORE2)
	hSens1.init();
	hSens2.init();
	hSens3.init();
	hSens4.init();
	hSens5.init();
	hSens6.init();

	hExtClass::hExtInit();
	hExt.init();

	hServoModuleClass::init();

	hBtn1.init();
	hBtn2.init();
	hCfg.init();
#elif BOARD(CORE2MINI)
	hSens1.init();
	hSens2.init();
	hSens3.init();

	hServoModuleClass::init();

	hBtn1.init();
	hCfg.init();
#else
#  error no board
#endif

	sys.delay(195);

#if BOARD(CORE2) || BOARD(CORE2MINI)
	pin5V.write(1);
#endif
	sys.delay(5); //delay - to provide stable 5V when hMain begin

	// enableAutoWatchdog();
}

inline bool isInterrupt()
{
	return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

uint32_t hSystem::getRandNr()
{
	// enable random number generator
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG->CR |= RNG_CR_RNGEN;
	while (!(RNG->SR & RNG_SR_DRDY));
	return RNG->DR;
}

uint32_t hSystem::getUsTimVal()
{
	return myTimerGetCnt();
}

float hSystem::getSupplyVoltage()
{
#if BOARD(ROBOCORE)
	const int UPPER_RESISTOR = 100; // kOhm
	const int LOWER_RESISTOR = 20;  // kOhm
#elif BOARD(CORE2) || BOARD(CORE2MINI)
	const int UPPER_RESISTOR = 56;  // kOhm
	const int LOWER_RESISTOR = 10;  // kOhm
#else
#error
#endif

	float voltage = supplySensePin.analogReadVoltage();

	return voltage * (UPPER_RESISTOR + LOWER_RESISTOR) / LOWER_RESISTOR;
}

uint32_t hSystem::getSupplyVoltageMV()
{
	return (uint32_t)(getSupplyVoltage() * 1000);
}

void hSystem::startCritical()
{
	portENTER_CRITICAL();
}

void hSystem::endCritical()
{
	portEXIT_CRITICAL();
}

void* hSystem::malloc(uint32_t size)
{
	return ::malloc((size_t)size);
}

void hSystem::free(void * ptr)
{
	return ::free(ptr);
}

static int mallocLockCounter = 0;

extern "C" void __malloc_lock(void* unused)
{
	taskENTER_CRITICAL();
	if (mallocLockCounter == 0)
		vTaskSuspendAll();

	mallocLockCounter ++;
	taskEXIT_CRITICAL();
}

extern "C" void __malloc_unlock(void* unused)
{
	mallocLockCounter --;
	if (mallocLockCounter == 0)
		xTaskResumeAll();
}

extern "C" void* pvPortMalloc(size_t xWantedSize)
{
	return malloc(xWantedSize);
}

extern "C" void vPortFree(void* pv)
{
	free(pv);
}

// for newlib

char* hSystem::getTaskList(char* taskList)
{
	vTaskList(taskList);
	return taskList;
}

void* hSystem::getThisTaskHandle()
{
	return xTaskGetCurrentTaskHandle();
}

char* hSystem::getStats(char* stats)
{
	vTaskGetRunTimeStats(stats);
	return stats;
}

void hSystem::printStats(SortMode sortMode, hPrintfDev* dev)
{
	if (!dev)
		dev = logDev;

	int num = uxTaskGetNumberOfTasks();
	TaskStatus_t tasks[num];

	unsigned long totalTicks;
	num = uxTaskGetSystemState(tasks, num, &totalTicks);

	int (*sortFn)(const void*, const void*) = 0;
	switch (sortMode)
	{
	case SortMode::Name:
		sortFn = [](const void* p1, const void* p2) -> int
		{
			TaskStatus_t* task1 = (TaskStatus_t*)p1;
			TaskStatus_t* task2 = (TaskStatus_t*)p2;
			return strcmp(task1->pcTaskName, task2->pcTaskName);
		};
		break;
	case SortMode::Ticks:
		sortFn = [](const void* p1, const void* p2) -> int
		{
			TaskStatus_t* task1 = (TaskStatus_t*)p1;
			TaskStatus_t* task2 = (TaskStatus_t*)p2;
			return task2->ulRunTimeCounter - task1->ulRunTimeCounter;
		};
		break;
	case SortMode::None:
		break;
	}

	if (sortFn)
		qsort(tasks, num, sizeof(TaskStatus_t), sortFn);

	for (int i = 0; i < num; i++)
	{
		TaskStatus_t task = tasks[i];
		int ratio = task.ulRunTimeCounter * 100 / totalTicks;
		if (ratio == 0)
			dev->printf("%10s %8u   <1%%\r\n", task.pcTaskName, task.ulRunTimeCounter);
		else
			dev->printf("%10s %8u  %3d%%\r\n", task.pcTaskName, task.ulRunTimeCounter, ratio);
	}

	dev->printf("----------------------------\r\n");
	dev->printf("     TOTAL %8d (%d ms)\r\n", totalTicks, totalTicks / 168);
}

void hSystem::getUid(uint8_t* id)
{
	uint8_t* id_reg = (uint8_t*)0x1fff7A10;

	for (int i = 0; i < 6; i++)
		id[i] = id_reg[i];
}

void hSystem::getRobocoreKey(uint8_t* key)
{
	const TRoboCOREHeader *header = getRobocoreHeader();
	memcpy(key, header->key, sizeof(header->key));
}

void hSystem::reset()
{
	NVIC_SystemReset();
}

#if BOARD(ROBOCORE) || BOARD(CORE2)
void hSystem::enableUsbCharging()
{
	pinUsbCharging.write(1);
}
void hSystem::disableUsbCharging()
{
	pinUsbCharging.write(0);
}
#endif
void hSystem::disableAutoWatchdog()
{
}
void hSystem::enableAutoWatchdog()
{
	IWDG->KR = 0x5555;
	IWDG->PR = 0b101;
	IWDG->RLR = 128;
	sys.delay(1);
	// while(IWDG->SR & IWDG_SR_PVU) fail_log("%d\r\n", IWDG->SR); // this doesn't work, don't know why, known bug
	// while(IWDG->SR & IWDG_SR_RVU) fail_log("%d\r\n", IWDG->SR);
	IWDG->KR = 0xAAAA;
	IWDG->KR = 0xCCCC;
}

void hSystem::enableSyslog()
{
	syslogDev = &Serial;
}
void hSystem::disableSyslog()
{
	syslogDev = &DevNull;
}

void hSystem::setLogDev(hPrintfDev* dev)
{
	if (dev)
		logDev = dev;
	else
		logDev = &DevNull;
}

void hSystem::setSysLogDev(hPrintfDev* dev)
{
	if (dev)
		syslogDev = dev;
	else
		syslogDev = &DevNull;
}

int hSystem::log(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return vlog(format, arg);
}
int hSystem::vlog(const char* format, va_list arg)
{
	return logDev->vprintf(format, arg);
}

int hSystem::syslog(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return vsyslog(format, arg);
}
int hSystem::vsyslog(const char* format, va_list arg)
{
	return syslogDev->vprintf(format, arg);
}

hStorage& hSystem::getStorage()
{
	return storage;
}

int hSystem::fail_log(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return fail_vlog(format, arg);
}
void fail_putc(char c, void* user)
{
	myUSART_fail_putc(c);
}
int hSystem::fail_vlog(const char* format, va_list arg)
{
	hPrintfContext ctx(format, arg);
	ctx.putchar = &fail_putc;
	return ctx.doFormat();
}

void hSystem::fault_log(const char* msg)
{
	for (int i = 0; msg[i]; i ++)
		myUSART_fail_putc(msg[i]);
}

void hSystem::idleTask()
{
	while (1)
	{
#if BOARD(ROBOCORE)
		Usb.task();
#else
		sys.delay(100);
#endif
	}
}

void hSystem::tickHook()
{
	IWDG->KR = 0xAAAA;
}

void hSystem::fault_handler()
{
	taskDISABLE_INTERRUPTS();
	for (uint32_t i = 0;; i++)
	{
		sys_delay_ms_spin(70);
		hLED1.set(i & 1);
		hLED2.set(!(i & 1));
		hLED3.set(i & 2);
	}
}

#if BOARD(ROBOCORE)
hLegoSensor& hSystem::getSensor(int num)
#else
hSensor& hSystem::getSensor(int num)
#endif
{
	switch (num)
	{
	default:
#if BOARD(CORE2)
	case 1: return hSens1;
	case 2: return hSens2;
	case 3: return hSens3;
	case 4: return hSens4;
	case 5: return hSens5;
	case 6: return hSens6;
#elif BOARD(CORE2MINI)
	case 1: return hSens1;
	case 2: return hSens2;
	case 3: return hSens3;
#elif BOARD(ROBOCORE)
	case 1: return hSens1;
	case 2: return hSens2;
	case 3: return hSens3;
	case 4: return hSens4;
	case 5: return hSens5;
#else
#  error no board
#endif
	}
}
hMotor& hSystem::getMotor(int num)
{
	switch (num)
	{
	default:
#if BOARD(CORE2)
	case 1: return hMotA;
	case 2: return hMotB;
	case 3: return hMotC;
	case 4: return hMotD;
#elif BOARD(CORE2MINI)
	case 1: return hMotA;
	case 2: return hMotB;
#elif BOARD(ROBOCORE)
	case 1: return hMotA;
	case 2: return hMotB;
	case 3: return hMotC;
	case 4: return hMotD;
	case 5: return hMotE;
#else
#  error no board
#endif
	}
}

// TRoboCOREHeader
bool TRoboCOREHeader::isClear() const
{
	uint8_t* ptr = (uint8_t*)this;
	for (std::size_t i = 0; i < sizeof(TRoboCOREHeader); i++)
		if (*ptr++ != 0xff)
			return false;
	return true;
}

bool TRoboCOREHeader::isValid() const
{
	if (headerVersion == 0x01)
	{
		return true;
	}
	else if (headerVersion == 0x02)
	{
		uint16_t crc = crc16_calc((uint8_t*)this, sizeof(TRoboCOREHeader) - 2);
		return crc == checksum;
	}
	return false;
}

bool TRoboCOREHeader::isKeyValid() const
{
	if (key[0] != 0x01)
		return false;

	uint16_t expectedCrc = crc16_calc(key + 1, 16);
	uint16_t calcCrc;
	memcpy(&calcCrc, (key + 17), 2);

	return expectedCrc == calcCrc;
}

void TRoboCOREHeader::calcChecksum()
{
	checksum = crc16_calc((uint8_t*)this, sizeof(TRoboCOREHeader) - 2);
}

const uint32_t OTP_START = 0x1fff7800;
const TRoboCOREHeader* getRobocoreHeader()
{
	for (int block_i = 0; block_i < 4; block_i ++)
	{
		const uint32_t OTP_BASE = OTP_START + 32 * block_i;

		const TRoboCOREHeader* header = (const TRoboCOREHeader*)OTP_BASE;

		if (!header->isClear() && header->isValid() && header->isKeyValid())
		{
			return header;
		}
	}
	return 0;
}

}
