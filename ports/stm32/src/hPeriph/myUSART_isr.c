/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myUSART_common.h"
#include "myUSART_isr.h"
#include "myUSART.h"
#include "port.h"
#include "debug.h"
#include "system.h"
#include "ycm.h"

extern uint32_t baudrate_[];
extern EUartParity parity_[];
extern EUartStopBits stopbits_[];
extern EUartFlowControl flowcontrol_[];
extern usart_current_config current_config_tx[];
extern usart_current_config current_config_rx[];
extern xSemaphoreHandle usartTxSemphr[];
extern xSemaphoreHandle usartRxSemphr[];

const uint16_t UsartBufferLength = 512;

extern usart_config_t usarts[];
xQueueHandle queueTX[MYUSART_NUMBER_OF_USARTS];
xQueueHandle queueRX[MYUSART_NUMBER_OF_USARTS];
xSemaphoreHandle semphrTXdone[MYUSART_NUMBER_OF_USARTS];

#define USART_TC(_usart) (*((volatile uint32_t*)(BITBAND_PERIPHERAL(&(_usart.usart->CR1),6))))
#define USART_TXE(_usart) (*((volatile uint32_t*)(BITBAND_PERIPHERAL(&(_usart.usart->CR1),7))))
#define USART_RXNE(_usart) (*((volatile uint32_t*)(BITBAND_PERIPHERAL(&(_usart.usart->CR1),5))))

void myUSART_config_isr_tx(uint8_t nr)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (queueTX[nr] == NULL)
	{
		queueTX[nr] = xQueueCreate(UsartBufferLength, 1);
		ASSERT(queueTX[nr]);
	}
	xQueueReset(queueTX[nr]);

	if (semphrTXdone[nr] == NULL)
	{
		vSemaphoreCreateBinary(semphrTXdone[nr]);
		ASSERT(semphrTXdone[nr]);
	}
	xSemaphoreTake(semphrTXdone[nr], 0);

	USART_Cmd(usarts[nr].usart, DISABLE);

	USART_ITConfig(usarts[nr].usart, USART_IT_PE, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_TC, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ORE_RX, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_IDLE, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_LBD, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_CTS, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ERR, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ORE_ER, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_NE, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].usart_irqType;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(usarts[nr].usart, ENABLE);
}

void myUSART_config_isr_rx(uint8_t nr)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (queueRX[nr] == NULL)
	{
		queueRX[nr] = xQueueCreate(UsartBufferLength, 1);
		ASSERT(queueRX[nr]);
	}

	xQueueReset(queueRX[nr]);

	USART_Cmd(usarts[nr].usart, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, ENABLE);

	USART_ITConfig(usarts[nr].usart, USART_IT_PE, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_TC, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ORE_RX, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_IDLE, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_LBD, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_CTS, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ERR, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_ORE_ER, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_NE, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].usart_irqType;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, ENABLE);

	USART_Cmd(usarts[nr].usart, ENABLE);
}


int myUSART_write_isr(uint8_t nr, const void* data, uint32_t size, uint32_t timeout)
{
	// todo: proper handling timeout (only for first byte)
	if (size == 0)
		return 0;
	uint32_t i;
	uint32_t retVal = 0;
	// ASSERT(size < 0x7fffffff);
	if (pdTRUE == xSemaphoreTake(usartTxSemphr[nr], msToTicks(timeout)))
	{
		for (i = 0; i < size; i++)
		{
			if (xQueueSendToBack(queueTX[nr], (uint8_t*)data + i, 0) != pdTRUE)
				break;
			retVal++;
		}
		USART_TXE(usarts[nr]) = 1;
		USART_TC(usarts[nr]) = 1; // TC must be after TXE enabling

		if (xSemaphoreTake(semphrTXdone[nr], timeout) == pdTRUE)
		{
			xSemaphoreGive(usartTxSemphr[nr]);
			return retVal;
		}
		else
		{
			xSemaphoreGive(usartTxSemphr[nr]);
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int myUSART_read_isr(uint8_t nr, void* data, uint32_t size, uint32_t timeout)
{
	uint32_t i;

	if (pdTRUE == xSemaphoreTake(usartRxSemphr[nr], msToTicks(timeout)))
	{
		if (current_config_rx[nr] != USART_CURRENT_CONFIG_ISR)
		{
			myUSART_config_isr_rx(nr);
			current_config_rx[nr] = USART_CURRENT_CONFIG_ISR;
		}

		for (i = 0; i < size; i++)
		{
			// if (uxQueueMessagesWaiting(queueRX[nr]) < (uint16_t)UsartBufferLength)
			// {
				// USART_RXNE(usarts[nr]) = 1;
			// }

			if (xQueueReceive(queueRX[nr], (uint8_t*)data + i, timeout) == pdFALSE)
				break;
			timeout = 0; // timeout is only for receiving first byte
		}
		xSemaphoreGive(usartRxSemphr[nr]);
		return i;
	}
	else
	{
		return -1;
	}
}

uint8_t myUSART_flushRXbuf_isr(uint8_t nr)
{
	if (current_config_rx[nr] != USART_CURRENT_CONFIG_ISR)
	{
		return 0;
	}

	return xQueueReset(queueRX[nr]) == pdTRUE;
}

uint32_t myUSART_getReadBufCnt_isr(uint8_t nr)
{
	if (USART_CURRENT_CONFIG_ISR != current_config_rx[nr])
	{
		return 0;
	}

	return (uint32_t)uxQueueMessagesWaiting(queueRX[nr]);
}

int myUSART_waitForData(uint8_t nr, uint32_t timeout)
{
	if (USART_CURRENT_CONFIG_ISR != current_config_rx[nr])
	{
		return 0;
	}

	uint8_t d;
	return xQueuePeek(queueRX[nr], &d, msToTicks(timeout)) == pdTRUE;
}

static inline void usartIrqHandler(uint8_t nr)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint8_t byte;

	/* USART in Receiver mode */
	if ((usarts[nr].usart->SR & USART_SR_RXNE) && (usarts[nr].usart->CR1 & USART_CR1_RXNEIE))
	{
		byte = usarts[nr].usart->DR;
		xQueueSendToBackFromISR(queueRX[nr], &byte, &xHigherPriorityTaskWoken);
	}
	if ((usarts[nr].usart->SR & USART_SR_ORE) && (usarts[nr].usart->CR1 & USART_CR1_RXNEIE))
	{
		// TODO: set overrun flag
		byte = usarts[nr].usart->DR;
		xQueueSendToBackFromISR(queueRX[nr], &byte, &xHigherPriorityTaskWoken);
	}

	/* USART in Tramitter mode */
	if ((usarts[nr].usart->SR & USART_SR_TXE) && (usarts[nr].usart->CR1 & USART_CR1_TXEIE))
	{
		if (xQueueReceiveFromISR(queueTX[nr], &byte, &xHigherPriorityTaskWoken) == pdTRUE)
		{
			usarts[nr].usart->DR = byte;
		}
		else
		{
			USART_TXE(usarts[nr]) = 0;
		}
	}
	if ((usarts[nr].usart->SR & USART_SR_TC) && (usarts[nr].usart->CR1 & USART_CR1_TCIE))
	{
		USART_TC(usarts[nr]) = 0;
		xSemaphoreGiveFromISR(semphrTXdone[nr], &xHigherPriorityTaskWoken);
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#define UART_IRQ(_uart, _id) \
	void _uart ## _IRQHandler(void) { usartIrqHandler(_id); }

#if BOARD(ROBOCORE)
UART_IRQ(USART1, MYUSART_FT)
UART_IRQ(USART6, MYUSART_EXT1)
UART_IRQ(USART2, MYUSART_EXT2)
UART_IRQ(UART5,  MYUSART_SENS3)
UART_IRQ(USART3, MYUSART_EDI)
#elif BOARD(CORE2)
UART_IRQ(USART3, MYUSART_FT)
UART_IRQ(USART2, MYUSART_EXT)
UART_IRQ(UART5,  MYUSART_SENS3)
UART_IRQ(USART6, MYUSART_SENS4)
UART_IRQ(USART1, MYUSART_RPI)
#elif BOARD(CORE2MINI)
UART_IRQ(USART3, MYUSART_FT)
UART_IRQ(USART6, MYUSART_SENS2)
UART_IRQ(USART2, MYUSART_SENS3)
UART_IRQ(USART1, MYUSART_RPI)
#else
#  error no board
#endif
