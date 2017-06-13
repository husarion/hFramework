/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSerial.h"
#include "myUSART.h"
#include "myGPIO.h"
#include "hUtils.h"
#include <hPrintf.h>

#include <stdbool.h>
#include "stm32f4xx.h"
#include <stdarg.h>

#include <hSystem.h>
#include <system.h>
#include "debug.h"
#include "ycm.h"

namespace hFramework
{

struct
{
	int implNo;
	uint8_t pinTx, pinRx, pinRtx, pinCts;
} const hSerialDefs[] =
{
#if BOARD(ROBOCORE)
	{ MYUSART_FT,    MYGPIO_FT_SERIAL_TX,  MYGPIO_FT_SERIAL_RX,  0, 0 },
	{ MYUSART_EXT1,  MYGPIO_EXT1_PIN9_UTX, MYGPIO_EXT1_PIN8_URX, 0, 0 },
	{ MYUSART_EXT2,  MYGPIO_EXT2_PIN9_UTX, MYGPIO_EXT2_PIN8_URX, 0, 0 },
	{ MYUSART_SENS3, MYGPIO_SENS3_DIGIA,   MYGPIO_SENS3_DIGIB,   0, 0 },
	{ MYUSART_EDI,   MYGPIO_EDI_SERIAL_TX, MYGPIO_EDI_SERIAL_RX, MYGPIO_EDI_SERIAL_RTS, MYGPIO_EDI_SERIAL_CTS },
#elif BOARD(CORE2)
	{ MYUSART_FT,    MYGPIO_FT_SERIAL_TX,  MYGPIO_FT_SERIAL_RX,  0, 0 },
	{ MYUSART_EXT,   MYGPIO_EXT_PIN7_UTX,  MYGPIO_EXT_PIN6_URX,  0, 0 },
	{ MYUSART_SENS3, MYGPIO_SENS3_DIGIC,   MYGPIO_SENS3_DIGIC,   0, 0 },
	{ MYUSART_SENS4, MYGPIO_SENS4_DIGIC,   MYGPIO_SENS4_DIGIC,   0, 0 },
	{ MYUSART_RPI,   MYGPIO_RPI_SERIAL_TX, MYGPIO_RPI_SERIAL_RX, 0, 0 },
#elif BOARD(CORE2MINI)
	{ MYUSART_FT,    MYGPIO_FT_SERIAL_TX,  MYGPIO_FT_SERIAL_RX,  0, 0 },
	{ MYUSART_SENS2, MYGPIO_SENS2_DIGIC,   MYGPIO_SENS2_DIGIC,   0, 0 },
	{ MYUSART_SENS3, MYGPIO_SENS3_DIGIC,   MYGPIO_SENS3_DIGIC,   0, 0 },
	{ MYUSART_RPI,   MYGPIO_RPI_SERIAL_TX, MYGPIO_RPI_SERIAL_RX, 0, 0 },
#else
#  error no board
#endif
};

class hSerial::Pimpl
{
public:
	uint8_t implNo;
	uint32_t baudRate;
	Parity parity;
	StopBits stopBits;
	// EUartFlowControl flow;
	bool initialized;
};

hSerial::hSerial(hSerial_ID no)
	: pinTx(hSerialDefs[no].pinTx),
	  pinRx(hSerialDefs[no].pinRx)
{
	pimpl = (hSerial::Pimpl*)sys.malloc(sizeof(hSerial::Pimpl));
	pimpl->initialized = false;
	pimpl->implNo = no;
}

void hSerial::init(uint32_t baudrate, Parity parity, StopBits stopBits)
{
	pimpl->parity = parity;
	pimpl->stopBits = stopBits;
	pimpl->baudRate = baudrate;
	// pimpl->flow = flow;

	myUSART_config(pimpl->implNo, pimpl->baudRate, (EUartParity)pimpl->parity, (EUartStopBits)pimpl->stopBits, FLOW_CONTROL_NONE);
	// myUSART_set_mode(pimpl->implNo, USART_CURRENT_CONFIG_ISR, USART_CURRENT_CONFIG_ISR);
}

int hSerial::write(const void* data, int len, uint32_t timeout)
{
	if (len == 0)
		return 0;
	int retVal = myUSART_write(pimpl->implNo, data, len, timeout);
	return retVal;
}
int hSerial::read(void* data, int len, uint32_t timeout)
{
	int retVal = myUSART_read(pimpl->implNo, data, len, timeout);
	return retVal;
}

void hSerial::setBaudrate(uint32_t baudrate)
{
	pimpl->baudRate = baudrate;
	myUSART_config(pimpl->implNo, pimpl->baudRate, (EUartParity)pimpl->parity, (EUartStopBits)pimpl->stopBits, FLOW_CONTROL_NONE);
}
void hSerial::flushRx()
{
	myUSART_flushRXbuf_isr(pimpl->implNo);
}

uint32_t hSerial::available()
{
	return myUSART_getReadBufCnt_isr(pimpl->implNo);
}

bool hSerial::waitForData(uint32_t timeout)
{
	return myUSART_waitForData(pimpl->implNo, timeout);
}

void hSerial::selectGPIO()
{
	myUSART_releaseGPIO(pimpl->implNo);
	pinTx.init();
	pinRx.init();
}
void hSerial::selectSerial()
{
	myUSART_acquireGPIO(pimpl->implNo);
}

}
