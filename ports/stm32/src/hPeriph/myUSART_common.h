/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYUSART_COMMON_H
#define MYUSART_COMMON_H

#include "hTypes.h"

#include "stm32f4xx.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

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
	uint8_t dma_nr;
	uint8_t dma_stream_nr;
	uint32_t DMA_IT_TCIFx;
	uint32_t DMA_IT_TEIFx;
	uint32_t DMA_IT_DMEIFx;
	uint32_t DMA_IT_FEIFx;
} DMA_direction_t;

typedef struct
{
	GPIO_config_t tx;
	GPIO_config_t rx;
	GPIO_config_t rts;
	GPIO_config_t cts;

	USART_TypeDef* usart;
	uint32_t usart_clock;
	void (*usart_clock_cmd)(uint32_t , FunctionalState);
	IRQn_Type usart_irqType;

	//for DMA
	// DMA_TypeDef* DMAx;
	// uint32_t dma_clock;
	// DMA_direction_t tx_DMA_config;
	// DMA_direction_t rx_DMA_config;

} usart_config_t;



#ifdef __cplusplus
}
#endif

#endif
