/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSERIAL_H__
#define __HSERIAL_H__

#include <stdarg.h>

#include "ISerial.h"

namespace hFramework
{

enum hSerial_ID
{
#if BOARD(ROBOCORE)
	hSerial_ID_FT,
	hSerial_ID_EXT1,
	hSerial_ID_EXT2,
	hSerial_ID_SENS3,
	hSerial_ID_EDISON,
#elif BOARD(CORE2)
	hSerial_ID_FT,
	hSerial_ID_EXT,
	hSerial_ID_SENS3,
	hSerial_ID_SENS4,
	hSerial_ID_RPI,
#elif BOARD(CORE2MINI)
	hSerial_ID_FT,
	hSerial_ID_SENS2,
	hSerial_ID_SENS3,
	hSerial_ID_RPI,
#else
#  error no board
#endif
	hSerial_ID_INVALID,
};

/**
 * @brief Serial port (UART) interface.
 *
 * You should never need to construct this class directly.
 * Use \c Serial and \c Edison variables.
 * Serials for hExts are in hExt::serial variables (e.g. \c hExt1.serial)
 */
class hSerial : public virtual ISerial
{
	friend class hExt;

public:
	hGPIO pinTx, pinRx;

	// Private constructor
	hSerial(hSerial_ID no);

	/**
	 * @brief Initialize serial port.
	 * @param baudrate a baudrate value
	 * @param upar an UART parity value (NONE, ODD, EVEN)
	 * @param ustopb usart stop bits (ONE, TWO)
	 */
	void init(uint32_t baudrate = 460800, Parity parity = Parity::None, StopBits stopBits = StopBits::One);

	int write(const void* data, int len, uint32_t timeout = INFINITE) override;
	int read(void* data, int len, uint32_t timeout = INFINITE) override;

	void setBaudrate(uint32_t baudrate) override;
	void flushRx() override;
	uint32_t available() override;

	bool waitForData(uint32_t timeout);

	void selectGPIO();
	void selectSerial();

private:
	class Pimpl;
	Pimpl* pimpl;
	void fromFriendConfig(hSerial_ID no);

	hSerial(const hSerial&) = delete;
};

}

#endif
