/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ISERIAL_H__
#define __ISERIAL_H__

#include <stdarg.h>

#include <hTypes.h>
#include <hGPIO.h>
#include <hPrintfDev.h>

namespace hFramework
{

/**
 * @brief Interface to the serial port.
 */
class ISerial : public virtual hPrintfDev
{
public:
	ISerial() { }
	virtual ~ISerial() { }

	virtual void setBaudrate(uint32_t baudrate) = 0;

	virtual void flushRx() = 0; //!< Flush the RX queue.

	/**
	 * @brief Return number of bytes waiting in the RX queue.
	 * @return bytes waiting in RX queue.
	 */
	virtual uint32_t available() = 0;
};

}

#endif
