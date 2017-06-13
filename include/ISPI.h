/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ISPI_H__
#define __ISPI_H__

#include <stdint.h>

namespace hFramework
{

/**
 * @brief Interface for SPI.
 */
class ISPI
{
public:
	virtual bool write(void* data, uint32_t len) = 0;
	virtual bool read(void* data, uint32_t len) = 0;
	virtual bool rw(void* dataTx, void* dataRx, uint32_t len) = 0;
};

}

#endif
