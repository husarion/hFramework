/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSERIAL_H__
#define __HSERIAL_H__

#include "hPrintfDev.h"

namespace hFramework
{

enum hSerial_ID
{
    hSerial_uart0 = 0,
    hSerial_uart1 = 1,
    hSerial_uart2 = 2
};

/**
 * @brief Serial port (UART) interface.
 */
class hSerial : public virtual hPrintfDev
{
public:
    // Private constructor
    hSerial(hSerial_ID no);

    /**
     * @brief Initialize serial port.
     * @param baudrate a baudrate value
     * @param upar an UART parity value (NONE, ODD, EVEN)
     * @param ustopb usart stop bits (ONE, TWO)
     */
    void init(uint32_t baudrate = 230400, Parity parity = Parity::None, StopBits stopBits = StopBits::One);

    int write(const void* data, int len, uint32_t timeout = INFINITE) override;
    int read(void* data, int len, uint32_t timeout = INFINITE) override;
private:
    hSerial(const hSerial&) = delete;
    hSerial_ID no;
};

extern hSerial Serial1;
}
#endif
