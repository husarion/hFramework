/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "myUSART.h"
#include "system.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "myGPIO.h"
#include "myEXTI.h"

#include "debug.h"

#include <hTypes.h>

#define DEFINE_EXTI(name) name,

static const int8_t extiPin[16] =
{
#include "myGPIO_include.h"
};

struct exti_data_t
{
	xSemaphoreHandle semHandle;
	std::function<void()> handler;

	exti_data_t() : semHandle(0) { }
} exti_data[16];

uint8_t getEXTI_nr(uint8_t pinNr)
{
	for (int i = 0; i < 16; i++)
		if ((int8_t)pinNr == extiPin[i])
			return i;
	ASSERT(0);
	return 0xff;
}

uint8_t myEXTI_waitIntrpt(uint8_t pinNr, uint32_t timeout)
{
	int extiNr = getEXTI_nr(pinNr);
	ASSERT(extiNr < 16);
	ASSERT(extiPin[extiNr] >= 0);
	exti_data_t& data = exti_data[extiNr];
	return xSemaphoreTake(data.semHandle, msToTicks(timeout)) == pdTRUE;
}

void myEXTI_config(uint8_t pinNr, sem_type_t semphrType, const std::function<void()>& handler)
{
	int extiNr = getEXTI_nr(pinNr);
	ASSERT(extiNr < 16);
	ASSERT(extiPin[extiNr] >= 0);
	exti_data_t& data = exti_data[extiNr];

	data.handler = handler;

	if (data.semHandle != 0)
	{
		vSemaphoreDelete(data.semHandle);
		data.semHandle = 0;
	}

	switch (semphrType)
	{
	case SEMPHR_NORMAL:   data.semHandle = xSemaphoreCreateBinary();         break;
	case SEMPHR_COUNTING: data.semHandle = xSemaphoreCreateCounting(255, 0); break;
	default: break;
	}
	ASSERT(data.semHandle != 0);
	xSemaphoreTake(data.semHandle, 0);
}

static void PROCESS_EXTI_LINE(int extiNr)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	exti_data_t& data = exti_data[extiNr];
	uint32_t EXTI_Line_bit = 1 << extiNr; // EXTI_Line0,1,2...
	if (EXTI_GetITStatus(EXTI_Line_bit) != RESET)
	{
		data.handler();
		if (data.semHandle)
			xSemaphoreGiveFromISR(data.semHandle, &xHigherPriorityTaskWoken);
		EXTI_ClearITPendingBit(EXTI_Line_bit);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

extern "C" void EXTI0_IRQHandler(void)
{
	PROCESS_EXTI_LINE(0);
}
extern "C" void EXTI1_IRQHandler(void)
{
	PROCESS_EXTI_LINE(1);
}
extern "C" void EXTI2_IRQHandler(void)
{
	PROCESS_EXTI_LINE(2);
}
extern "C" void EXTI3_IRQHandler(void)
{
	PROCESS_EXTI_LINE(3);
}
extern "C" void EXTI4_IRQHandler(void)
{
	PROCESS_EXTI_LINE(4);
}
extern "C" void EXTI9_5_IRQHandler(void)
{
	PROCESS_EXTI_LINE(5);
	PROCESS_EXTI_LINE(6);
	PROCESS_EXTI_LINE(7);
	PROCESS_EXTI_LINE(8);
	PROCESS_EXTI_LINE(9);
}
extern "C" void EXTI15_10_IRQHandler(void)
{
	PROCESS_EXTI_LINE(10);
	PROCESS_EXTI_LINE(11);
	PROCESS_EXTI_LINE(12);
	PROCESS_EXTI_LINE(13);
	PROCESS_EXTI_LINE(14);
	PROCESS_EXTI_LINE(15);
}
