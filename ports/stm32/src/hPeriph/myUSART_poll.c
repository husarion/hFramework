/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myUSART_common.h"
#include "myUSART_poll.h"
#include "myUSART.h"
#include "debug.h"

extern uint32_t baudrate_[];
extern EUartParity parity_[];
extern EUartStopBits stopbits_[];
extern EUartFlowControl flowcontrol_[];
extern usart_current_config current_config_tx[];
extern usart_current_config current_config_rx[];
extern xSemaphoreHandle usartTxSemphr[];
extern xSemaphoreHandle usartRxSemphr[];

extern usart_config_t usarts[];

void myUSART_config_poll_tx(uint8_t nr)
{
	USART_Cmd(usarts[nr].usart, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_TXE, DISABLE);
	// DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_TC, DISABLE);
	// DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_TE, DISABLE);
	// DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_DME, DISABLE);
	// DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_FE, DISABLE);
	USART_Cmd(usarts[nr].usart, ENABLE);
}

void myUSART_config_poll_rx(uint8_t nr)
{
	USART_Cmd(usarts[nr].usart, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, DISABLE);
	// DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_TC, DISABLE);
	// DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_TE, DISABLE);
	// DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_DME, DISABLE);
	// DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_FE, DISABLE);
	USART_Cmd(usarts[nr].usart, ENABLE);
}

int myUSART_write_poll(uint8_t nr, const void* data, uint32_t size, uint32_t timeout)
{
	uint32_t i;

	if (pdTRUE == xSemaphoreTake(usartTxSemphr[nr] , timeout))
	{
		if (USART_CURRENT_CONFIG_POLL != current_config_tx[nr])
		{
			myUSART_config_poll_tx(nr);
			current_config_tx[nr] = USART_CURRENT_CONFIG_POLL;
		}
		for (i = 0; i < size; i++)
		{
			USART_SendData(usarts[nr].usart, *(((uint8_t*)data) + i));
			while (USART_GetFlagStatus(usarts[nr].usart, USART_FLAG_TC) == RESET)
			{
				;
			}
		}
		xSemaphoreGive(usartTxSemphr[nr]);
		// ASSERT(i < 0x7fffffff);
		return (int)i;
	}
	else
	{
		return -1;
	}
}

int myUSART_read_poll(uint8_t nr, void* data, uint32_t size, uint32_t timeout)
{
	uint32_t i;

	if (pdTRUE == xSemaphoreTake(usartRxSemphr[nr] , timeout))
	{
		if (USART_CURRENT_CONFIG_POLL != current_config_rx[nr])
		{
			myUSART_config_poll_rx(nr);
			current_config_rx[nr] = USART_CURRENT_CONFIG_POLL;
		}

		for (i = 0; i < size; i++)
		{
			while (RESET == USART_GetFlagStatus(usarts[nr].usart, USART_FLAG_RXNE))
			{
				;
			}
			*(((uint8_t*)data) + i) = (uint8_t) USART_ReceiveData(usarts[nr].usart);
		}

		xSemaphoreGive(usartRxSemphr[nr]);
		// ASSERT(size < 0x7fffffff);
		return (int)size;
	}
	else
	{
		return -1;
	}
}
