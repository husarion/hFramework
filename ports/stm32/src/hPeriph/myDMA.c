/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "myUSART.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define DEBUG_DMA 1

xSemaphoreHandle semphrDMA1;
xSemaphoreHandle semphrDMA2;

#include "system.h"
#include "debug.h"

#if 0 == DEBUG_DMA
#define debugDMA(fmt, args...)
#else
#define debugDMA(fmt, args...) sys_log("\r\n%s:   "fmt, __FUNCTION__,##args);
#endif

xSemaphoreHandle DMA_Smphr[2][8];

void debugAll(DMA_Stream_TypeDef* DMAx_Streamy, uint8_t x, uint8_t y)
{
	if (1 == x)
	{
		debugDMA("DMA1->LISR = 0x%x", DMA1->LISR);
		debugDMA("DMA1->HISR = 0x%x", DMA1->HISR);
	}
	else
	{
		debugDMA("DMA2->LISR = 0x%x", DMA2->LISR);
		debugDMA("DMA2->HISR = 0x%x", DMA2->HISR);
	}
	debugDMA("DMA%d_Stream%d->NDTR = 0x%x", x, y, DMAx_Streamy->NDTR);
	debugDMA("DMA%d_Stream%d->PAR = 0x%x", x, y, DMAx_Streamy->PAR);
	debugDMA("DMA%d_Stream%d->M0AR = 0x%x", x, y, DMAx_Streamy->M0AR);
	debugDMA("DMA%d_Stream%d->M1AR = 0x%x", x, y, DMAx_Streamy->M1AR);
	debugDMA("DMA%d_Stream%d->FCR = 0x%x", x, y, DMAx_Streamy->FCR);
	debugDMA("DMA%d_Stream%d->CR = 0x%x", x, y, DMAx_Streamy->CR);
}

uint8_t errorFlagUsart = 0;

void DMA_handler(DMA_Stream_TypeDef* DMAx_Streamy,
                 uint8_t x, uint8_t y,
                 uint32_t DMA_IT_TCIFy,
                 uint32_t DMA_IT_TEIFy,
                 uint32_t DMA_IT_DMEIFy,
                 uint32_t DMA_IT_FEIFy ,
                 signed portBASE_TYPE* xHigherPriorityTaskWoken)
{
	*xHigherPriorityTaskWoken = pdFALSE;
	// Transfer completed
	if (DMA_GetITStatus(DMAx_Streamy, DMA_IT_TCIFy) == SET)
	{

		xSemaphoreGiveFromISR(DMA_Smphr[x - 1][y], xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(*xHigherPriorityTaskWoken);

		//Clear DMA Stream Transfer Complete interrupt pending bit
		DMA_ClearITPendingBit(DMAx_Streamy, DMA_IT_TCIFy);
		return;
	}
	if (DMA_GetITStatus(DMAx_Streamy, DMA_IT_TEIFy) == SET)
	{
//		errorFlagUsart = 1;
//		xSemaphoreGiveFromISR(DMA_Smphr[x-1][y], xHigherPriorityTaskWoken);
//
//		portYIELD_FROM_ISR(*xHigherPriorityTaskWoken);
//
//		//Clear DMA Stream Transfer Complete interrupt pending bit
//		DMA_ClearITPendingBit(DMAx_Streamy,DMA_IT_TEIFy);
//		return;

		debugAll(DMAx_Streamy, x, y);
		DMAx_Streamy->CR |= DMA_SxCR_EN;
		ASSERT2(0, "DMA%d_Stream%d - transfer error", x, y);
		DMA_ClearITPendingBit(DMAx_Streamy, DMA_IT_TEIFy);
		//	DMAx_Streamy->CR |= DMA_SxCR_EN;
	}
	if (DMA_GetITStatus(DMAx_Streamy, DMA_IT_DMEIFy) == SET)
	{
		debugAll(DMAx_Streamy, x, y);
		ASSERT2(0, "DMA%d_Stream%d - direct mode error error", x, y);
		DMA_ClearITPendingBit(DMAx_Streamy, DMA_IT_DMEIFy);
	}
	if (DMA_GetITStatus(DMAx_Streamy, DMA_IT_FEIFy) == SET)
	{
		debugAll(DMAx_Streamy, x, y);
		ASSERT2(0, "DMA%d_Stream%d - fifo error", x, y);
		DMA_ClearITPendingBit(DMAx_Streamy, DMA_IT_FEIFy);
	}
}

//===============================DMA1=====================================
void DMA1_Stream0_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream0, 1, 0, DMA_IT_TCIF0, DMA_IT_TEIF0, DMA_IT_DMEIF0, DMA_IT_FEIF0, &xHigherPriorityTaskWoken);
}
void DMA1_Stream1_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream1, 1, 1, DMA_IT_TCIF1, DMA_IT_TEIF1, DMA_IT_DMEIF1, DMA_IT_FEIF1, &xHigherPriorityTaskWoken);
}

void DMA1_Stream2_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream2, 1, 2, DMA_IT_TCIF2, DMA_IT_TEIF2, DMA_IT_DMEIF2, DMA_IT_FEIF2, &xHigherPriorityTaskWoken);
}
void DMA1_Stream3_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream3, 1, 3, DMA_IT_TCIF3, DMA_IT_TEIF3, DMA_IT_DMEIF3, DMA_IT_FEIF3, &xHigherPriorityTaskWoken);
}
void DMA1_Stream4_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream4, 1, 4, DMA_IT_TCIF4, DMA_IT_TEIF4, DMA_IT_DMEIF4, DMA_IT_FEIF4, &xHigherPriorityTaskWoken);
}

void DMA1_Stream5_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream5, 1, 5, DMA_IT_TCIF5, DMA_IT_TEIF5, DMA_IT_DMEIF5, DMA_IT_FEIF5, &xHigherPriorityTaskWoken);
}
void DMA1_Stream6_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream6, 1, 6, DMA_IT_TCIF6, DMA_IT_TEIF6, DMA_IT_DMEIF6, DMA_IT_FEIF6, &xHigherPriorityTaskWoken);
}

void DMA1_Stream7_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA1_Stream7, 1, 7, DMA_IT_TCIF7, DMA_IT_TEIF7, DMA_IT_DMEIF7, DMA_IT_FEIF7, &xHigherPriorityTaskWoken);
}

//===============================DMA2=====================================
void DMA2_Stream0_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream0, 2, 0, DMA_IT_TCIF0, DMA_IT_TEIF0, DMA_IT_DMEIF0, DMA_IT_FEIF0, &xHigherPriorityTaskWoken);
}

void DMA2_Stream1_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream1, 2, 1, DMA_IT_TCIF1, DMA_IT_TEIF1, DMA_IT_DMEIF1, DMA_IT_FEIF1, &xHigherPriorityTaskWoken);
}
//void DMA2_Stream1_IRQHandler(void)
//{
//	static signed portBASE_TYPE xHigherPriorityTaskWoken;
//	xHigherPriorityTaskWoken = pdFALSE;
//	// Transfer completed
//	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET) {
//
//		xSemaphoreGiveFromISR(DMA_Smphr[1][1], &xHigherPriorityTaskWoken);
//		if(xHigherPriorityTaskWoken == pdTRUE) {
//			taskYIELD();
//		}
//		//Clear DMA Stream Transfer Complete interrupt pending bit
//		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
//		return;
//	}
//}
void DMA2_Stream2_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream2, 2, 2, DMA_IT_TCIF2, DMA_IT_TEIF2, DMA_IT_DMEIF2, DMA_IT_FEIF2, &xHigherPriorityTaskWoken);
}
void DMA2_Stream3_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream3, 2, 3, DMA_IT_TCIF3, DMA_IT_TEIF3, DMA_IT_DMEIF3, DMA_IT_FEIF3, &xHigherPriorityTaskWoken);
}
void DMA2_Stream4_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream4, 2, 4, DMA_IT_TCIF4, DMA_IT_TEIF4, DMA_IT_DMEIF4, DMA_IT_FEIF4, &xHigherPriorityTaskWoken);
}
void DMA2_Stream5_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream5, 2, 5, DMA_IT_TCIF5, DMA_IT_TEIF5, DMA_IT_DMEIF5, DMA_IT_FEIF5, &xHigherPriorityTaskWoken);
}
void DMA2_Stream6_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream6, 2, 6, DMA_IT_TCIF6, DMA_IT_TEIF6, DMA_IT_DMEIF6, DMA_IT_FEIF6, &xHigherPriorityTaskWoken);
}
void DMA2_Stream7_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	DMA_handler(DMA2_Stream7, 2, 7, DMA_IT_TCIF7, DMA_IT_TEIF7, DMA_IT_DMEIF7, DMA_IT_FEIF7, &xHigherPriorityTaskWoken);
}
