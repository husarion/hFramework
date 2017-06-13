/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hUtils.h"
#include "hSystem.h"
#include "hPrintf.h"
#include "hSerial.h"

#include <string.h>

namespace hFramework
{

uint16_t crc16_update(uint16_t crc, uint8_t data)
{
	int i;

	crc ^= data;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}

	return crc;
}
uint16_t crc16_update_block(uint16_t crc, const void* data, int length)
{
	const uint8_t* buf = (uint8_t*)data;
	for (int i = 0; i < length; i++)
		crc = crc16_update(crc, *buf++);
	return crc;
}
uint16_t crc16_calc(const void* data, int length)
{
	return crc16_update_block(0, data, length);
}

}

