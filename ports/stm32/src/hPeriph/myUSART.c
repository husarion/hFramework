/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//#include "hUtils.h"
#include "myUSART_common.h"

#include "myUSART_poll.h"
#include "myUSART_isr.h"
// #include "myUSART_dma.h"
#include "myUSART.h"

#include "hTypes.h"
#include "port.h"
#include "debug.h"

#define USART_ENTRY_FLOW(gpioTX, pinTX, gpioRX, pinRX, gpioRTS, pinRTS, gpioCTS, pinCTS, usart, apb) \
	{ { GPIO_Pin_ ## pinTX, gpioTX, RCC_AHB1Periph_ ## gpioTX, GPIO_PinSource ## pinTX, GPIO_AF_ ## usart }, \
	  { GPIO_Pin_ ## pinRX, gpioRX, RCC_AHB1Periph_ ## gpioRX, GPIO_PinSource ## pinRX, GPIO_AF_ ## usart }, \
	  { GPIO_Pin_ ## pinRTS, gpioRTS, RCC_AHB1Periph_ ## gpioRTS, GPIO_PinSource ## pinRTS, GPIO_AF_ ## usart }, \
	  { GPIO_Pin_ ## pinCTS, gpioCTS, RCC_AHB1Periph_ ## gpioCTS, GPIO_PinSource ## pinCTS, GPIO_AF_ ## usart }, \
		usart, \
		RCC_ ## apb ## Periph_ ## usart, \
		RCC_ ## apb ## PeriphClockCmd, \
		usart ## _IRQn }

#define USART_ENTRY(gpioTX, pinTX, gpioRX, pinRX, usart, apb) \
	{ { GPIO_Pin_ ## pinTX, gpioTX, RCC_AHB1Periph_ ## gpioTX, GPIO_PinSource ## pinTX, GPIO_AF_ ## usart }, \
	  { GPIO_Pin_ ## pinRX, gpioRX, RCC_AHB1Periph_ ## gpioRX, GPIO_PinSource ## pinRX, GPIO_AF_ ## usart }, \
	  { 0, 0, 0, 0, 0 }, \
	  { 0, 0, 0, 0, 0 }, \
		usart, \
		RCC_ ## apb ## Periph_ ## usart, \
		RCC_ ## apb ## PeriphClockCmd, \
		usart ## _IRQn }

const usart_config_t usarts[] =
{
#if BOARD(ROBOCORE)
	/* MYUSART_FT    */ USART_ENTRY(GPIOA,  9, GPIOA, 10, USART1, APB2),
	/* MYUSART_EXT1  */ USART_ENTRY(GPIOG, 14, GPIOG,  9, USART6, APB2),
	/* MYUSART_EXT2  */ USART_ENTRY(GPIOD,  5, GPIOD,  6, USART2, APB1),
	/* MYUSART_SENS3 */ USART_ENTRY(GPIOC, 12, GPIOD,  2,  UART5, APB1),
	/* MYUSART_EDI   */ USART_ENTRY_FLOW(GPIOB, 10, GPIOB, 11, GPIOB, 14, GPIOD, 11, USART3, APB1),
#elif BOARD(CORE2)
	/* MYUSART_FT    */ USART_ENTRY(GPIOB, 10, GPIOB, 11, USART3, APB1),
	/* MYUSART_EXT   */ USART_ENTRY(GPIOD,  5, GPIOD,  6, USART2, APB1),
	/* MYUSART_SENS3 */ USART_ENTRY(GPIOC, 12, GPIOD,  2,  UART5, APB1),
	/* MYUSART_SENS4 */ USART_ENTRY(GPIOG,  9, GPIOG, 14, USART6, APB2),
	/* MYUSART_RPI   */ USART_ENTRY(GPIOA,  9, GPIOA, 10, USART1, APB2),
#elif BOARD(CORE2MINI)
	/* MYUSART_FT    */ USART_ENTRY(GPIOB, 10, GPIOB, 11, USART3, APB1),
	/* MYUSART_SENS2 */ USART_ENTRY(GPIOC,  6, GPIOC,  7, USART6, APB2),
	/* MYUSART_SENS3 */ USART_ENTRY(GPIOA,  2, GPIOA,  3, USART2, APB1),
	/* MYUSART_RPI   */ USART_ENTRY(GPIOA,  9, GPIOA, 10, USART1, APB2),
#else
#  error no board
#endif
};


uint32_t baudrate_[MYUSART_NUMBER_OF_USARTS];
EUartParity parity_[MYUSART_NUMBER_OF_USARTS];
EUartStopBits stopbits_[MYUSART_NUMBER_OF_USARTS];
EUartFlowControl flowcontrol_[MYUSART_NUMBER_OF_USARTS];
usart_current_config current_config_tx[MYUSART_NUMBER_OF_USARTS];
usart_current_config current_config_rx[MYUSART_NUMBER_OF_USARTS];
xSemaphoreHandle usartTxSemphr[MYUSART_NUMBER_OF_USARTS];
xSemaphoreHandle usartRxSemphr[MYUSART_NUMBER_OF_USARTS];

void myUSART_config(uint8_t nr, uint32_t baudrate, EUartParity parity, EUartStopBits stopbits, EUartFlowControl flowcontrol)
{
	current_config_tx[nr] = USART_CURRENT_CONFIG_POLL;
	current_config_rx[nr] = USART_CURRENT_CONFIG_POLL;
	flowcontrol_[nr] = flowcontrol;
	baudrate_[nr] = baudrate;
	parity_[nr] = parity;
	stopbits_[nr] = stopbits;

	USART_InitTypeDef USART_InitStructure;
	// GPIO_InitTypeDef GPIO_InitStructure;

	if (0 == usartTxSemphr[nr])
	{
		vSemaphoreCreateBinary(usartTxSemphr[nr]);
		ASSERT(0 != usartTxSemphr[nr]);
	}
	if (0 == usartRxSemphr[nr])
	{
		vSemaphoreCreateBinary(usartRxSemphr[nr]);
		ASSERT(0 != usartRxSemphr[nr]);
	}

	USART_Cmd(usarts[nr].usart, DISABLE);

	/* Peripheral Clock Enable -------------------------------------------------*/
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(usarts[nr].tx.gpio_clock | usarts[nr].rx.gpio_clock, ENABLE);

	/* Enable USART clock */
	usarts[nr].usart_clock_cmd(usarts[nr].usart_clock, ENABLE);

	/* USARTx configuration ----------------------------------------------------*/
	/* Enable the USART OverSampling by 8 */
	USART_OverSampling8Cmd(usarts[nr].usart, ENABLE);

	/* USARTx configured as follow:
	 - BaudRate = 5250000 baud
	 - Maximum BaudRate that can be achieved when using the Oversampling by 8
	 is: (USART APB Clock / 8)
	 Example:
	 - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
	 - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
	 - Maximum BaudRate that can be achieved when using the Oversampling by 16
	 is: (USART APB Clock / 16)
	 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
	 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
	 - Word Length = 8 Bits
	 - one Stop Bit
	 - No parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 */
	USART_InitStructure.USART_BaudRate = baudrate_[nr];
	switch (stopbits_[nr])
	{
	case ONE:
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		break;
	case TWO:
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
		break;
	}

	/* When using Parity the word length must be configured to 9 bits */
	switch (parity_[nr])
	{
	case NONE:
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	case EVEN:
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	case ODD:
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	}

	if ((flowcontrol_[nr] == FLOW_CONTROL_RTS_CTS)
	    && (usarts[nr].rts.pin != 0) && (usarts[nr].cts.pin != 0))
	{
		RCC_AHB1PeriphClockCmd(usarts[nr].rts.gpio_clock, ENABLE);
		RCC_AHB1PeriphClockCmd(usarts[nr].cts.gpio_clock, ENABLE);

		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	}
	else
	{
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	}

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(usarts[nr].usart, &USART_InitStructure);

	myUSART_config_isr_tx(nr);
	current_config_tx[nr] = USART_CURRENT_CONFIG_ISR;
	myUSART_config_isr_rx(nr);
	current_config_rx[nr] = USART_CURRENT_CONFIG_ISR;

	myUSART_acquireGPIO(nr);
}
void myUSART_acquireGPIO(uint8_t nr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinAFConfig(usarts[nr].tx.port, usarts[nr].tx.gpio_pin_source, usarts[nr].tx.gpio_af);
	GPIO_PinAFConfig(usarts[nr].rx.port, usarts[nr].rx.gpio_pin_source, usarts[nr].rx.gpio_af);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = usarts[nr].tx.pin;
	GPIO_Init(usarts[nr].tx.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = usarts[nr].rx.pin;
	GPIO_Init(usarts[nr].rx.port, &GPIO_InitStructure);

	if ((flowcontrol_[nr] == FLOW_CONTROL_RTS_CTS)
	    && (usarts[nr].rts.pin != 0) && (usarts[nr].cts.pin != 0))
	{
		GPIO_PinAFConfig(usarts[nr].rts.port, usarts[nr].rts.gpio_pin_source, usarts[nr].rts.gpio_af);
		GPIO_PinAFConfig(usarts[nr].cts.port, usarts[nr].cts.gpio_pin_source, usarts[nr].cts.gpio_af);

		GPIO_InitStructure.GPIO_Pin = usarts[nr].rts.pin;
		GPIO_Init(usarts[nr].rts.port, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = usarts[nr].cts.pin;
		GPIO_Init(usarts[nr].cts.port, &GPIO_InitStructure);
	}
}
void myUSART_releaseGPIO(uint8_t nr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = usarts[nr].tx.pin;
	GPIO_Init(usarts[nr].tx.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = usarts[nr].rx.pin;
	GPIO_Init(usarts[nr].rx.port, &GPIO_InitStructure);

	if ((flowcontrol_[nr] == FLOW_CONTROL_RTS_CTS)
	    && (usarts[nr].rts.pin != 0) && (usarts[nr].cts.pin != 0))
	{
		GPIO_InitStructure.GPIO_Pin = usarts[nr].rts.pin;
		GPIO_Init(usarts[nr].rts.port, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = usarts[nr].cts.pin;
		GPIO_Init(usarts[nr].cts.port, &GPIO_InitStructure);
	}
}

// void myUSART_disable_TX(uint8_t nr)
// {
// GPIO_InitTypeDef GPIO_InitStructure;

// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

// GPIO_InitStructure.GPIO_Pin = usarts[nr].tx.pin;
// GPIO_Init(usarts[nr].tx.port, &GPIO_InitStructure);
// }

// void myUSART_enable_TX(uint8_t nr)
// {
// GPIO_InitTypeDef GPIO_InitStructure;

// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

// GPIO_InitStructure.GPIO_Pin = usarts[nr].tx.pin;
// GPIO_Init(usarts[nr].tx.port, &GPIO_InitStructure);
// }

int myUSART_write(uint8_t nr, const void* data, uint32_t size, uint32_t timeout)
{
	switch (current_config_tx[nr])
	{
	case USART_CURRENT_CONFIG_POLL:
		return 0;
		return myUSART_write_poll(nr, data, size, timeout);
	case USART_CURRENT_CONFIG_ISR:
		return myUSART_write_isr(nr, data, size, timeout);
	// case USART_CURRENT_CONFIG_DMA:
		// return myUSART_write_dma(nr, data, size, timeout);
	default:
		return myUSART_write_poll(nr, data, size, timeout);
	}
}

int myUSART_read(uint8_t nr, void* data, uint32_t size, uint32_t timeout)
{
	switch (current_config_rx[nr])
	{
	case USART_CURRENT_CONFIG_POLL:
		return myUSART_read_poll(nr, data, size, timeout);
	case USART_CURRENT_CONFIG_ISR:
		return myUSART_read_isr(nr, data, size, timeout);
	// case USART_CURRENT_CONFIG_DMA:
		// return myUSART_read_dma(nr, data, size, timeout);
	default:
		return myUSART_read_poll(nr, data, size, timeout);
	}
}

// void myUSART_set_mode(uint8_t nr, usart_current_config RxMode,
// usart_current_config TxMode)
// {
// switch (TxMode)
// {
// case USART_CURRENT_CONFIG_DMA:
// myUSART_write_dma(nr, 0, 0, 0);
// break;
// case USART_CURRENT_CONFIG_ISR:
// myUSART_write_isr(nr, 0, 0, 0);
// break;
// case USART_CURRENT_CONFIG_POLL:
// myUSART_write_poll(nr, 0, 0, 0);
// break;
// case USART_CURRENT_CONFIG_NONE:
// myUSART_disable_TX(nr);
// break;
// }
// switch (RxMode)
// {
// case USART_CURRENT_CONFIG_DMA:
// myUSART_read_dma(nr, 0, 0, 0);
// break;
// case USART_CURRENT_CONFIG_ISR:
// myUSART_read_isr(nr, 0, 0, 0);
// break;
// case USART_CURRENT_CONFIG_POLL:
// myUSART_read_poll(nr, 0, 0, 0);
// break;
// }
// }

int myUSART_fail_putc(char c)
{
	USART_TypeDef *uart;
#if BOARD(ROBOCORE)
	uart = USART1;
#elif BOARD(CORE2)
	uart = USART3;
#elif BOARD(CORE2MINI)
	uart = USART3;
#else
#  error no board
#endif
	USART_SendData(uart, c);
	while (USART_GetFlagStatus(uart, USART_FLAG_TC) == RESET);
	return 1;
}
