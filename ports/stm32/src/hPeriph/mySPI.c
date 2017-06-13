/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "mySPI.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "debug.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

extern xSemaphoreHandle DMA_Smphr[2][8];

typedef struct
{
	uint16_t pin;
	GPIO_TypeDef* port;
	uint32_t gpio_clock;
	uint8_t gpio_pin_source;
	uint8_t gpio_af;
} GPIO_config_t;

typedef struct
{
	uint32_t DMA_Channel_x;
	DMA_Stream_TypeDef *DMAx_StreamY;
	IRQn_Type DMAx_StreamY_IRQn;
	uint32_t DMA_FLAG_TCIFy;
} DMA_direction_t;

typedef struct
{
	uint8_t dma_nr;
	uint8_t dma_rx_stream_nr;

	GPIO_config_t miso;
	GPIO_config_t mosi;
	GPIO_config_t sclk;

	//for SPI
	SPI_TypeDef*  spi;
	uint32_t spi_clock;
	void (*RCC_APB_X_PeriphClockCmd)(uint32_t, FunctionalState);

	//for DMA
	uint32_t DR_address;
	uint32_t DMA_IT_TCIFx_for_TX;
	DMA_TypeDef* DMAx;
	uint32_t dma_clock;

	DMA_direction_t tx;
	DMA_direction_t rx;

	//other
	xSemaphoreHandle rxTCsmphr;
	//uint8_t mode;	//0 - taki jaki ma WiFi, 1 - taki jaki ma CC1120
} SPI_config_t;


SPI_config_t mySPI3 =
{
	1,	//DMA nr
	0,	//DMA rx stream NR

#if BOARD(ROBOCORE)
	{GPIO_Pin_11, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource11, GPIO_AF_SPI3},	//MISO
	{GPIO_Pin_5, GPIOB, RCC_AHB1Periph_GPIOB, GPIO_PinSource5, GPIO_AF_SPI3},	//MOSI
	{GPIO_Pin_10, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource10, GPIO_AF_SPI3},	//SCLK
#elif BOARD(CORE2)
	{GPIO_Pin_11, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource11, GPIO_AF_SPI3},	//MISO
	{GPIO_Pin_5, GPIOB, RCC_AHB1Periph_GPIOB, GPIO_PinSource5, GPIO_AF_SPI3},	//MOSI
	{GPIO_Pin_10, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource10, GPIO_AF_SPI3},	//SCLK
#elif BOARD(CORE2MINI)
	{GPIO_Pin_11, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource11, GPIO_AF_SPI3},	//MISO
	{GPIO_Pin_12, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource12, GPIO_AF_SPI3},	//MOSI
	{GPIO_Pin_10, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource10, GPIO_AF_SPI3},	//SCLK
#else
#  error no board
#endif

	//for SPI
	SPI3,	//SPI
	RCC_APB1Periph_SPI3,	//SPI clk
	RCC_APB1PeriphClockCmd,	//SPI clk init funtion

	//for DMA
	(uint32_t)&(SPI3->DR),	//DR address
	DMA_IT_TCIF7,
	DMA1,			//DMAx
	RCC_AHB1Periph_DMA1,		//DMA clk init function

	{DMA_Channel_0, DMA1_Stream7, DMA1_Stream7_IRQn, DMA_FLAG_TCIF7},	//DMA TX
	{DMA_Channel_0, DMA1_Stream0, DMA1_Stream0_IRQn, DMA_FLAG_TCIF0},	//DMA RX

	NULL,	//rx TC semaphore
	//1		//mode 1 - CC2530
};

SPI_config_t mySPI2 =
{
	1,
	3,

#if BOARD(ROBOCORE)
	{GPIO_Pin_2, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource2, GPIO_AF_SPI2},	//MISO
	{GPIO_Pin_15, GPIOB, RCC_AHB1Periph_GPIOB, GPIO_PinSource15, GPIO_AF_SPI2},	//MOSI
	{GPIO_Pin_13, GPIOB, RCC_AHB1Periph_GPIOB, GPIO_PinSource13, GPIO_AF_SPI2},	//SCLK
#elif BOARD(CORE2)
	{GPIO_Pin_2, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource2, GPIO_AF_SPI2},	//MISO
	{GPIO_Pin_3, GPIOC, RCC_AHB1Periph_GPIOC, GPIO_PinSource3, GPIO_AF_SPI2},	//MOSI
	{GPIO_Pin_13, GPIOB, RCC_AHB1Periph_GPIOB, GPIO_PinSource13, GPIO_AF_SPI2},	//SCLK
#elif BOARD(CORE2MINI)
	{0,0,0,0,0},
#else
#  error no board
#endif

	//for SPI
	SPI2,
	RCC_APB1Periph_SPI2,
	RCC_APB1PeriphClockCmd,

	//for DMA
	(uint32_t)&(SPI2->DR),
	DMA_IT_TCIF4,
	DMA1,
	RCC_AHB1Periph_DMA1,

	{DMA_Channel_0, DMA1_Stream4, DMA1_Stream4_IRQn, DMA_FLAG_TCIF4},
	{DMA_Channel_0, DMA1_Stream3, DMA1_Stream3_IRQn, DMA_FLAG_TCIF3},

	NULL,
	//0
};

SPI_config_t* getSPI(int spiNum)
{
	switch (spiNum)
	{
	case 2: return &mySPI2;
	case 3: return &mySPI3;
	default: ASSERT(0);
	}
}

void mySPI_Config(uint8_t spiNum, uint8_t mode, uint16_t BaudRatePrescaler)
{
	SPI_config_t* spi = getSPI(spiNum);
	xSemaphoreHandle sem;

	SPI_InitTypeDef SPI_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if (NULL == DMA_Smphr[(spi->dma_nr) - 1][spi->dma_rx_stream_nr])
	{
		vSemaphoreCreateBinary(DMA_Smphr[(spi->dma_nr) - 1][spi->dma_rx_stream_nr]);
	}
	else
	{
		// TODO: report error
	}

	sem = DMA_Smphr[(spi->dma_nr) - 1][spi->dma_rx_stream_nr];
	spi->rxTCsmphr = sem;

	if (pdTRUE == xSemaphoreTake(sem, 10))
	{
		// TODO: ???
	}
	else
	{
		// TODO: report error
	}

	/*	Peripheral Clock Enable ---------------------------------------------- */
	/*	Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(spi->sclk.gpio_clock | spi->miso.gpio_clock | spi->mosi.gpio_clock, ENABLE);

	/*	Enable the SPI clock */

	spi->RCC_APB_X_PeriphClockCmd(spi->spi_clock, ENABLE);

	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(spi->dma_clock, ENABLE);



	/*	SPI configuration ----------------------------------------------------- */

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	switch (mode)
	{
	case 0:	//wifi
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case 1: //cc1120
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// SPI_CPHA_2Edge;
		break;
	case 2: //cc1120
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// SPI_CPHA_2Edge;
		break;
	case 3: //cc1120
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// SPI_CPHA_2Edge;
		break;
	}

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = BaudRatePrescaler;	//F_PCLK / 16 = 2.6MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	/*	Initializes the SPI communication */
	SPI_Init(spi->spi, &SPI_InitStructure);
	spi->spi->CR1 &= ~(0x2000);		//disable CRC checking

	/*	The Data transfer is performed in the DMA interrupt routine */
	/*	Enable the SPI peripheral */
	SPI_Cmd(spi->spi, ENABLE);

	/*	RX DMA configuration -------------------------------------------------- */

	/* Enable the CC1120_SPI RX DMA request */
	SPI_DMACmd(spi->spi, SPI_DMAReq_Rx | SPI_DMAReq_Tx, DISABLE);

	DMA_InitStructure.DMA_Channel = spi->rx.DMA_Channel_x;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buf1;
	//	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = spi->DR_address;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(spi->rx.DMAx_StreamY, &DMA_InitStructure);
	/*	TX DMA configuration -------------------------------------------------- */

	DMA_DeInit(spi->tx.DMAx_StreamY);
	DMA_InitStructure.DMA_Channel = spi->tx.DMA_Channel_x;;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buf2;
	//	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(spi->tx.DMAx_StreamY, &DMA_InitStructure);

	/* DMA IRQ configuration --------------------------------------------------- */

	/* Enable the DMA SPI RX interrupt (Transfer complete) */
	DMA_ITConfig(spi->rx.DMAx_StreamY, DMA_IT_TC, ENABLE);

	/* 	Configure the SPI DMA RX interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = spi->rx.DMAx_StreamY_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the CC1120_SPI RX DMA request */
	SPI_DMACmd(spi->spi, SPI_DMAReq_Rx | SPI_DMAReq_Tx, ENABLE);

	mySPI_acquireGPIO(spiNum);

}
void mySPI_acquireGPIO(uint8_t spiNum)
{
	SPI_config_t* spi = getSPI(spiNum);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(spi->sclk.port, spi->sclk.gpio_pin_source, spi->sclk.gpio_af);
	GPIO_PinAFConfig(spi->mosi.port, spi->mosi.gpio_pin_source, spi->mosi.gpio_af);
	GPIO_PinAFConfig(spi->miso.port, spi->miso.gpio_pin_source, spi->miso.gpio_af);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = spi->sclk.pin;
	GPIO_Init(spi->sclk.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = spi->mosi.pin;
	GPIO_Init(spi->mosi.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = spi->miso.pin;
	GPIO_Init(spi->miso.port, &GPIO_InitStructure);
}
void mySPI_releaseGPIO(uint8_t spiNum)
{
	SPI_config_t* spi = getSPI(spiNum);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = spi->sclk.pin;
	GPIO_Init(spi->sclk.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = spi->mosi.pin;
	GPIO_Init(spi->mosi.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = spi->miso.pin;
	GPIO_Init(spi->miso.port, &GPIO_InitStructure);
}

uint8_t bufTX[2][1400];
uint8_t bufRX[2][1400];

void mySPI_ReadWrite(uint8_t spiNum, uint8_t* tx_buf, uint8_t* rx_buf, uint32_t size)
{
	SPI_config_t* spi;
	xSemaphoreHandle sem;
	uint32_t i;

	GPIO_InitTypeDef GPIO_InitStructure;

	uint8_t* bufTX_ptr = bufTX[spiNum - 2];
	uint8_t* bufRX_ptr = bufRX[spiNum - 2];

	if (0 == size)
	{
		return;
	}

	switch (spiNum)
	{
	case 1:
		break;
	case 2:
		spi = &mySPI2;
		break;
	case 3:
		spi = &mySPI3;
		break;
	default:
		while (1)
		{
			;//error
		}
		break;
	}

	sem = DMA_Smphr[(spi->dma_nr) - 1][spi->dma_rx_stream_nr];

	//prepare RX
	DMA_ClearFlag(spi->rx.DMAx_StreamY, spi->rx.DMA_FLAG_TCIFy);

	DMA_Cmd(spi->rx.DMAx_StreamY, DISABLE);
	while ((spi->rx.DMAx_StreamY->CR & (uint32_t)DMA_SxCR_EN) != 0) {;}
	SPI_DMACmd(spi->spi, SPI_DMAReq_Rx, DISABLE);

	// Check whether DMA can be used
	if ((uint32_t)rx_buf & 0x10000000)  	// DMA can't use this region
	{
		spi->rx.DMAx_StreamY->M0AR = (uint32_t)bufRX_ptr;
	}
	else
	{
		spi->rx.DMAx_StreamY->M0AR = (uint32_t)rx_buf;

	}
	//===============================================================
	spi->rx.DMAx_StreamY->NDTR = size;

	if (0 == rx_buf)  	//only transmit mode
	{
		//disable memory increment mode for RX
		spi->rx.DMAx_StreamY->CR &= ~(DMA_SxCR_MINC);
		spi->rx.DMAx_StreamY->M0AR = (uint32_t)bufRX_ptr;
	}

	SPI_DMACmd(spi->spi, SPI_DMAReq_Rx, ENABLE);

	SPI_I2S_ReceiveData(spi->spi); // TODO: dopiero to ustawilo flage SPI_FLAG_RXNE na 0

	DMA_Cmd(spi->rx.DMAx_StreamY, ENABLE);

	//send Data===========
	DMA_ClearFlag(spi->tx.DMAx_StreamY, spi->tx.DMA_FLAG_TCIFy);

	DMA_Cmd(spi->tx.DMAx_StreamY, DISABLE);
	while ((spi->tx.DMAx_StreamY->CR & (uint32_t)DMA_SxCR_EN) != 0) {;}
	SPI_DMACmd(spi->spi, SPI_DMAReq_Tx, DISABLE);

	if ((uint32_t)tx_buf & 0x10000000) // DMA doesn't work in this region
	{
		for (i = 0; i < size; i++)
		{
			*(bufTX_ptr + i) = *(tx_buf + i);
		}
		spi->tx.DMAx_StreamY->M0AR = (uint32_t)bufTX_ptr;
	}
	else
	{
		spi->tx.DMAx_StreamY->M0AR = (uint32_t)tx_buf;
	}
	spi->tx.DMAx_StreamY->NDTR = size;

	if ((uint32_t)tx_buf <= 0xff) // only transmit mode
	{
		//disable memory increment mode for RX
		spi->tx.DMAx_StreamY->CR &= ~(DMA_SxCR_MINC);
		*(bufTX_ptr) = (uint8_t)(uint32_t)tx_buf;
		spi->tx.DMAx_StreamY->M0AR = (uint32_t)bufTX_ptr;
	}

	SPI_DMACmd(spi->spi, SPI_DMAReq_Tx, ENABLE);
	DMA_Cmd(spi->tx.DMAx_StreamY, ENABLE);

	if (pdTRUE == xSemaphoreTake(sem, 400))
	{
		;
	}
	else
	{
		while (1) {;}
	}

	DMA_ClearITPendingBit(spi->tx.DMAx_StreamY, spi->DMA_IT_TCIFx_for_TX); // TODO
	while (SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_TXE) == RESET) {}
	while (SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_BSY) == SET) {}
	//====================

	if ((uint32_t)rx_buf & 0x10000000) // DMA doesn't work in this region
	{
		for (i = 0; i < size; i++)
		{
			*(rx_buf + i) = *(bufRX_ptr + i);
		}
	}

	spi->rx.DMAx_StreamY->CR |= DMA_SxCR_MINC;
	spi->tx.DMAx_StreamY->CR |= DMA_SxCR_MINC;
}
