/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hStreamDev.h"

#include <hSystem.h>
#include <hUtils.h>
#include <cstdio>

namespace hFramework
{

int hStreamDev::readAll(void* data, int len, uint32_t timeout)
{
	uint8_t* _data = (uint8_t*)data;
	int pos = 0;
	uint32_t startTime = sys.getRefTime();
	bool firstIter = true;
	while (len)
	{
		uint32_t elapsed = firstIter ? 0 : (sys.getRefTime() - startTime);
		if (timeout != INFINITE && elapsed > timeout)
			break;
		firstIter = false;

		uint32_t toWait;
		if (timeout == INFINITE)
			toWait = INFINITE;
		else
			toWait = max((int32_t)0, (int32_t)timeout - (int32_t)elapsed);

		int ret = read(_data, len, toWait);
		if (ret < 0)
			return ret;
		if (ret == 0)
			break;

		_data += ret;
		pos += ret;
		len -= ret;
	}

	return pos;
}

int hStreamDev::writeAll(const void* data, int len, uint32_t timeout)
{
	uint8_t* _data = (uint8_t*)data;
	int pos = 0;
	uint32_t startTime = sys.getRefTime();
	while (len)
	{
		uint32_t elapsed = sys.getRefTime() - startTime;
		if (timeout != INFINITE && elapsed > timeout)
			break;

		uint32_t toWait;
		if (timeout == INFINITE)
			toWait = INFINITE;
		else
			toWait = max((int32_t)0, (int32_t)timeout - (int32_t)elapsed);

		int ret = write(_data, len, toWait);
		if (ret < 0)
			return ret;
		_data += ret;
		pos += ret;
		len -= ret;
	}
	return pos;
}


}
