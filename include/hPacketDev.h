/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HPACKETDEV_H__
#define __HPACKETDEV_H__

class hPacketDev
{
public:
	enum class Result { Success = 0, Timeout = -1, Error = -2 };

	virtual ~hPacketDev() { }

	virtual int readPacket(void* data, int maxLength, int timeout) = 0;

	virtual bool beginPacket() = 0;
	virtual bool write(const void* data, int length) = 0;
	virtual bool endPacket() = 0;
};

#endif
