/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myUSART_common.h"
#include "myUSART_dma.h"
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

extern xSemaphoreHandle DMA_Smphr[2][8];

static readHandler usartReadHandlers[3] = {0, 0, 0};

//=======================================================================================

void debugAll2(DMA_Stream_TypeDef* DMAx_Streamy, uint8_t x, uint8_t y)
{
	if (1 == x)
	{
		LOG("DMA1->LISR = 0x%x", DMA1->LISR);
		LOG("DMA1->HISR = 0x%x", DMA1->HISR);
	}
	else
	{
		LOG("DMA2->LISR = 0x%x", DMA2->LISR);
		LOG("DMA2->HISR = 0x%x", DMA2->HISR);
	}
	LOG("DMA%d_Stream%d->NDTR = 0x%x", x, y, DMAx_Streamy->NDTR);
	LOG("DMA%d_Stream%d->PAR = 0x%x", x, y, DMAx_Streamy->PAR);
	LOG("DMA%d_Stream%d->M0AR = 0x%x", x, y, DMAx_Streamy->M0AR);
	LOG("DMA%d_Stream%d->M1AR = 0x%x", x, y, DMAx_Streamy->M1AR);
	LOG("DMA%d_Stream%d->FCR = 0x%x", x, y, DMAx_Streamy->FCR);
	LOG("DMA%d_Stream%d->CR = 0x%x", x, y, DMAx_Streamy->CR);
}
//=======================================================================================
void myUSART_config_dma_tx(uint8_t nr)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (NULL == DMA_Smphr[usarts[nr].tx_DMA_config.dma_nr - 1][usarts[nr].tx_DMA_config.dma_stream_nr])
	{
		vSemaphoreCreateBinary(DMA_Smphr[usarts[nr].tx_DMA_config.dma_nr - 1][usarts[nr].tx_DMA_config.dma_stream_nr]);
		ASSERT(0 != DMA_Smphr[usarts[nr].tx_DMA_config.dma_nr - 1][usarts[nr].tx_DMA_config.dma_stream_nr]);
	}
	xSemaphoreTake(DMA_Smphr[usarts[nr].tx_DMA_config.dma_nr - 1][usarts[nr].tx_DMA_config.dma_stream_nr], 0);

	USART_Cmd(usarts[nr].usart, DISABLE);
	DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_TC, ENABLE);
	DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_TE, ENABLE);
	DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_DME, ENABLE);
	DMA_ITConfig(usarts[nr].tx_DMA_config.DMAx_StreamY, DMA_IT_FE, ENABLE);

	USART_ITConfig(usarts[nr].usart, USART_IT_TXE, DISABLE);

	/* 	Configure the SPI DMA TX interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].tx_DMA_config.DMAx_StreamY_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2; //2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	if (current_config_rx[nr] != USART_CURRENT_CONFIG_ISR)
	{
		NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].usart_irqType;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);

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
	}

	USART_Cmd(usarts[nr].usart, ENABLE);
}

void myUSART_config_dma_rx(uint8_t nr)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (NULL == DMA_Smphr[usarts[nr].rx_DMA_config.dma_nr - 1][usarts[nr].rx_DMA_config.dma_stream_nr])
	{
		vSemaphoreCreateBinary(DMA_Smphr[usarts[nr].rx_DMA_config.dma_nr - 1][usarts[nr].rx_DMA_config.dma_stream_nr]);
		ASSERT(0 != DMA_Smphr[usarts[nr].rx_DMA_config.dma_nr - 1][usarts[nr].rx_DMA_config.dma_stream_nr]);
	}
	xSemaphoreTake(DMA_Smphr[usarts[nr].rx_DMA_config.dma_nr - 1][usarts[nr].rx_DMA_config.dma_stream_nr], 0);

	USART_Cmd(usarts[nr].usart, DISABLE);
	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, ENABLE);
	DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_TC, ENABLE);
	DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_TE, ENABLE);
	DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_DME, ENABLE);
	DMA_ITConfig(usarts[nr].rx_DMA_config.DMAx_StreamY, DMA_IT_FE, ENABLE);

	USART_ITConfig(usarts[nr].usart, USART_IT_RXNE, DISABLE);

	/* 	Configure the SPI DMA TX interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].rx_DMA_config.DMAx_StreamY_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2; //2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	if (current_config_tx[nr] != USART_CURRENT_CONFIG_ISR)
	{
		NVIC_InitStructure.NVIC_IRQChannel = usarts[nr].usart_irqType;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);

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
	}

	USART_Cmd(usarts[nr].usart, ENABLE);
}

int myUSART_write_dma(uint8_t nr, const void* data, uint32_t size, uint32_t timeout)
{
	int i;
	static uint8_t bufTX_DMA[3][500];
	uint32_t retVal = 0;

	xSemaphoreHandle sem;
	DMA_Stream_TypeDef* DMAx_Streamy;

	ASSERT(size < 501);

	if (pdTRUE == xSemaphoreTake(usartTxSemphr[nr] , timeout))
	{
		if (USART_CURRENT_CONFIG_DMA != current_config_tx[nr])
		{
			myUSART_config_dma_tx(nr);
			current_config_tx[nr] = USART_CURRENT_CONFIG_DMA;
		}
		if (0 == size)
		{
			xSemaphoreGive(usartTxSemphr[nr]);
			return 0;
		}

		sem = DMA_Smphr[usarts[nr].tx_DMA_config.dma_nr - 1][usarts[nr].tx_DMA_config.dma_stream_nr];
		DMAx_Streamy = usarts[nr].tx_DMA_config.DMAx_StreamY;

		//ASSERT(pdTRUE == xSemaphoreTake(semphrDMA2,2000));

		//1. disable stream
		if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
		{
			DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
			while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
		}
		DMAx_Streamy->CR = 0;

		DMA_ClearITPendingBit(usarts[nr].tx_DMA_config.DMAx_StreamY, usarts[nr].tx_DMA_config.DMA_IT_TCIFx |
		                      usarts[nr].tx_DMA_config.DMA_IT_TEIFx |
		                      usarts[nr].tx_DMA_config.DMA_IT_DMEIFx |
		                      usarts[nr].tx_DMA_config.DMA_IT_FEIFx);

		//2. set data register
		DMAx_Streamy->PAR = (uint32_t) & (usarts[nr].usart->DR);

		//3. set memory address register
		if ((uint32_t)data & 0x10000000) // DMA doesn't work in this region
		{
			for (i = 0; i < size; i++)
			{
				bufTX_DMA[nr][i] = *(((uint8_t*)data) + i);
			}
			DMAx_Streamy->M0AR = (uint32_t)&bufTX_DMA[nr];
		}
		else
		{
			DMAx_Streamy->M0AR = (uint32_t)data;
		}

		//4. configure data items to be transferred
		DMAx_Streamy->NDTR = size;

		//5. select DMA channel request
		DMAx_Streamy->CR |= usarts[nr].tx_DMA_config.DMA_Channel_x;//DMA_Channel_5;

		//6. peripheral is flow controller
		//DMAx_Streamy->CR |= DMA_SxCR_PFCTRL;//todo

		//7. configure stream priority
		DMAx_Streamy->CR |= DMA_Priority_Medium; //- dzialalo dlugo i stabilnie

		//8. disable fifo - set direct mode
		DMAx_Streamy->FCR = 0;// | DMA_SxFCR_FEIE;

		//9. set all interrupts (except of half transfer)
		DMAx_Streamy->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;	//set all interrupts (except of half transfer)
		DMAx_Streamy->CR |= DMA_DIR_MemoryToPeripheral;	//set direction
		DMAx_Streamy->CR |= DMA_SxCR_MINC;				//set memory increment mode

		xSemaphoreTake(sem, 0);

		//10. activate the stream
		// USART_DMACmd(usarts[nr].usart, USART_DMAReq_Tx, ENABLE); // if this is enabled "DMA2_Stream7 - fifo error" happens
		DMAx_Streamy->CR |= DMA_SxCR_EN;
		USART_DMACmd(usarts[nr].usart, USART_DMAReq_Tx, ENABLE);

		if (pdTRUE == xSemaphoreTake(sem, timeout))
		{
			;
		}
		else
		{
			debugAll2(DMAx_Streamy, 2, 7);
			ASSERT(0);
			if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
			{
				DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
				while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
			}
			xSemaphoreGive(usartTxSemphr[nr]);
			return -1;
		}

		/* Waiting the end of Data transfer */
		// TODO: document why this is disabled
		// while (USART_GetFlagStatus(usarts[nr].usart,USART_FLAG_TC) == RESET);

		/* Clear USART Transfer Complete Flags */
		USART_ClearFlag(usarts[nr].usart, USART_FLAG_TC);
		USART_DMACmd(usarts[nr].usart, USART_DMAReq_Tx, DISABLE);

		if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
		{
			DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
			while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
		}

		xSemaphoreGive(usartTxSemphr[nr]);
		ASSERT(size < 0x7fffffff);
		return (int)size;
	}
	else
	{
		ASSERT(0);
		return -1;
	}
}

int myUSART_read_dma(uint8_t nr, void* data, uint32_t size, uint32_t timeout)
{
	int i;
	uint8_t bufRX_DMA[3][500];
	DMA_Stream_TypeDef* DMAx_Streamy;
	xSemaphoreHandle sem;

	ASSERT(size < 501);

	if (pdTRUE == xSemaphoreTake(usartRxSemphr[nr] , timeout))
	{
		if (USART_CURRENT_CONFIG_DMA != current_config_rx[nr])
		{
			myUSART_config_dma_rx(nr);
			current_config_rx[nr] = USART_CURRENT_CONFIG_DMA;
		}

		if (0 == size)
		{
			xSemaphoreGive(usartRxSemphr[nr]);
			return 0;
		}

		sem = DMA_Smphr[usarts[nr].rx_DMA_config.dma_nr - 1][usarts[nr].rx_DMA_config.dma_stream_nr];
		DMAx_Streamy = usarts[nr].rx_DMA_config.DMAx_StreamY;

		//1. disable stream
		if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
		{
			DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
			while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
		}
		DMAx_Streamy->CR = 0;

		DMA_ClearITPendingBit(usarts[nr].rx_DMA_config.DMAx_StreamY, usarts[nr].rx_DMA_config.DMA_IT_TCIFx |
		                      usarts[nr].rx_DMA_config.DMA_IT_TEIFx |
		                      usarts[nr].rx_DMA_config.DMA_IT_DMEIFx |
		                      usarts[nr].rx_DMA_config.DMA_IT_FEIFx);

		//2. set data register
		DMAx_Streamy->PAR = (uint32_t) & (usarts[nr].usart->DR);

		//3. set memory address register
		if ((uint32_t)data & 0x10000000)  // DMA doesn't work in this region
		{
			DMAx_Streamy->M0AR = (uint32_t)&bufRX_DMA[nr];
		}
		else
		{
			DMAx_Streamy->M0AR = (uint32_t)data;
		}

		//4. configure data items to be transferred
		DMAx_Streamy->NDTR = size;

		//5. select DMA channel request
		DMAx_Streamy->CR |= usarts[nr].rx_DMA_config.DMA_Channel_x;//DMA_Channel_5;

		//6. peripheral is flow controller
		//DMAx_Streamy->CR |= DMA_SxCR_PFCTRL;//todo

		//7. configure stream priority
		DMAx_Streamy->CR |= DMA_Priority_Medium;	// high

		//8. disable fifo - set direct mode
		DMAx_Streamy->FCR = 0;// | DMA_SxFCR_FEIE;

		//9. set all interrupts (except of half transfer)
		DMAx_Streamy->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;	//set all interrupts (except of half transfer)
		DMAx_Streamy->CR |= DMA_DIR_PeripheralToMemory;	//set direction
		DMAx_Streamy->CR |= DMA_SxCR_MINC;				//set memory increment mode

		xSemaphoreTake(sem, 0);

		//10. activate the stream
		DMAx_Streamy->CR |= DMA_SxCR_EN;
		USART_DMACmd(usarts[nr].usart, USART_DMAReq_Rx, ENABLE);

		if (pdTRUE == xSemaphoreTake(sem, timeout)) //todo: timeout for data send
		{
			;
		}
		else
		{
			debugAll2(DMAx_Streamy, 2, 2);
			ASSERT(0);
			if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
			{
				DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
				while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
			}
			xSemaphoreGive(usartRxSemphr[nr]);
			return -1;
		}

		/* Clear USART Transfer Complete Flags */
		USART_ClearFlag(usarts[nr].usart, USART_FLAG_TC);
		USART_DMACmd(usarts[nr].usart, USART_DMAReq_Rx, DISABLE);

		if ((uint32_t)data & 0x10000000) // DMA doesn't work in this region
		{
			for (i = 0; i < size; i++)
			{
				*(((uint8_t*)data) + i) = bufRX_DMA[nr][i];
			}
		}

		if (0 != (DMA_SxCR_EN & DMAx_Streamy->CR))
		{
			DMAx_Streamy->CR &= ~(DMA_SxCR_EN);
			while (0 != (DMA_SxCR_EN & DMAx_Streamy->CR));
		}

		xSemaphoreGive(usartRxSemphr[nr]);
		ASSERT(size < 0x7fffffff);
		return (int)size;
	}
	else
	{
		ASSERT(0);
		return -1;
	}
}
