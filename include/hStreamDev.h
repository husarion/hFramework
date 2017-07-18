/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_STREAM_DEV
#define H_STREAM_DEV

#include "hTypes.h"
#include <stdarg.h>

namespace hFramework
{

/**
 * @brief Stream device interface.
 */
class hStreamDev
{
public:
	/**
	 * @brief Write data to the underlying device.
	 * @param data data to be written
	 * @param len number of bytes to write
	 * @param timeout maximum timeout of the operation
	 * @return Number of written bytes.
	 */
	virtual int write(const void* data, int len, uint32_t timeout = 0xffffffff) = 0;

	/**
	 * @brief Read data from the underlying device.
     * If less than `len` bytes is available then less might be returned.
	 * @param data data to be read
	 * @param len number of bytes to read
	 * @param timeout maximum timeout of the operation
	 * @return Number of read bytes.
	 */
	virtual int read(void* data, int len, uint32_t timeout = 0xffffffff) = 0;

	/**
	 * @brief Read data from the underlying device.
	 * @param data data to be read
	 * @param len number of bytes to read
	 * @param timeout maximum timeout of the operation
	 * @return Number of read bytes.
	-*/
	int readAll(void* data, int len, uint32_t timeout = 0xffffffff);

	/**
	 * @brief Write data to the underlying device.
	 * @param data data to be written
	 * @param len number of bytes to write
	 * @param timeout maximum timeout of the operation
	 * @return Number of written bytes.
	 */
	int writeAll(const void* data, int len, uint32_t timeout = 0xffffffff);

	/**
	 * @brief Write formatted string to the underlying device.
	 * @param str pointer to null terminated format string
	 * @param ... - optional arguments
	 * @return number of written characters
	 */
	int printf(const char* format, ...);

	/**
	 * @brief Write formatted string to the underlying device.
	 * @param str pointer to null terminated format string
	 * @param arg arguments as a va_list
	 * @return number of written characters
	 */
	virtual int vprintf(const char* format, va_list arg);

	/**
	 * @brief Put single character.
	 * @param ch character
	 */
	int putchar(char c);

	/**
	 * @brief Return single character.
	 * @return character.
	 */
	int getch();
};

using hPrintfDev = hStreamDev;

class _DevNull : public hStreamDev {
	int write(const void* data, int len, uint32_t timeout = 0xffffffff) {
		return len;
	}

	int read(void* data, int len, uint32_t timeout = 0xffffffff) {
		return 0;
	}
};

__attribute__((used)) static _DevNull DevNull;
__attribute__((used)) static _DevNull devNull;

}
#endif //H_STREAM_DEV
