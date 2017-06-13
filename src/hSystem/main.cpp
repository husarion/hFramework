/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

// #include "myUSART.h"
#include "hSystem.h"
// #include "FreeRTOS.h"
// #include "task.h"
#include "hSerial.h"
// #include "hUSB.h"

// #include "myGPIO.h"
// #include "myADC.h"

#include "hFramework.h"

#include <stdio.h>

#include "debug.h"

extern "C" {
	volatile const uint32_t bootflag __attribute__((section(".bootflag"))) = 0xDEADBEEF;
}

void hMain();

namespace hFramework
{

extern "C" void __cxa_pure_virtual(void)
{
	sys.fail_log("__cxa_pure_virtual\r\n");
	sys.fault_handler();
}

extern "C" int boot_disable_syslog __attribute__((weak)) = 0;

void portInit();

void sys_idle_task();

void hMainInit()
{
	if (!boot_disable_syslog)
		sys.enableSyslog();

#ifdef DEBUG
	sys.syslog("DEBUG MODE\r\n");
#endif

	sys.initRTOS();
	sys.init();

// #ifdef DEBUG
	// portPrintDebug();
	// RCC_ClocksTypeDef clkVals;

	// LOG("=======Start======");
	// if (bootflag == 0xDEADBEEF)
	// LOG("programmer used");
	// else
	// LOG("bootloader used");
	// RCC_GetClocksFreq(&clkVals);

	// LOG("SYSCLK_Frequency  = %d [Hz]", clkVals.SYSCLK_Frequency);
	// LOG("HCLK_Frequency    = %d [Hz]", clkVals.HCLK_Frequency);
	// LOG("PCLK1_Frequency   = %d [Hz]", clkVals.PCLK1_Frequency);
	// LOG("PCLK2_Frequency   = %d [Hz]", clkVals.PCLK2_Frequency);
	// LOG("===================================");
	// LOG("RCC_CR = 0x%x", RCC->CR);
	// LOG("RCC_PLLCFGR = 0x%x", RCC->PLLCFGR);
	// LOG("RCC_CFGR = 0x%x", RCC->CFGR);
	// LOG("RCC_CIR = 0x%x", RCC->CIR);
	// LOG("===================================");
// #endif

	setvbuf(stdin, 0, _IONBF, 0);

	sys.taskCreate(hMain, 2, 1000, "hMain");
}

}
