/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYUSART_H
#define MYUSART_H

#include "hTypes.h"
#include <stdbool.h>
#include <stdarg.h>

#include "myUSART_dma.h"
#include "myUSART_isr.h"
#include "myUSART_poll.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	USART_CURRENT_CONFIG_DMA,
	USART_CURRENT_CONFIG_ISR,
	USART_CURRENT_CONFIG_POLL,
	USART_CURRENT_CONFIG_NONE
} usart_current_config;

enum
{
#if BOARD(ROBOCORE)
	MYUSART_FT,
	MYUSART_EXT1,
	MYUSART_EXT2,
	MYUSART_SENS3,
	MYUSART_EDI,
#elif BOARD(CORE2)
	MYUSART_FT,
	MYUSART_EXT,
	MYUSART_SENS3,
	MYUSART_SENS4,
	MYUSART_RPI,
#elif BOARD(CORE2MINI)
	MYUSART_FT,
	MYUSART_SENS2,
	MYUSART_SENS3,
	MYUSART_RPI,
#else
#  error no board
#endif
};

void myUSART_config(uint8_t nr, uint32_t baudrate, EUartParity parity, EUartStopBits stopbits, EUartFlowControl flowcontrol);
void myUSART_acquireGPIO(uint8_t nr);
void myUSART_releaseGPIO(uint8_t nr);

// void myUSART_disable_TX(uint8_t nr);
// void myUSART_enable_TX(uint8_t nr);
// void myUSART_set_mode(uint8_t nr, usart_current_config RxMode, usart_current_config TxMode);

int myUSART_write(uint8_t nr, const void* data, uint32_t size, uint32_t timeout);
int myUSART_read(uint8_t nr, void* data, uint32_t size, uint32_t timeout);

int myUSART_fail_putc(char c);

#ifdef __cplusplus
}
#endif

#endif /* MYUSART_H */
