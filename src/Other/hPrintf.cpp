/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdarg.h>
#include <stdint.h>

#include "hPrintf.h"
#include "myprintf.h"

#ifdef STM32F4
#include "hSerial.h"
#include "hSystem.h"
#endif

namespace hFramework
{

hPrintfContext::hPrintfContext(const char* format, va_list inArg)
	: format(format), buffer(0), bufferLen(0)
{
	va_copy(arg, inArg);
	va_end(inArg);
}
hPrintfContext::hPrintfContext(const char* format, va_list inArg, char* buffer, int bufferLen)
	: format(format), buffer(buffer), bufferLen(bufferLen)
{
	va_copy(arg, inArg);
	va_end(inArg);

	user = this;
	putchar = [](char c, void* user)
	{
		hPrintfContext* self = (hPrintfContext*)user;
		self->buffer[self->bufferPos++] = c;
		if (self->bufferPos == self->bufferLen)
			self->flushBuffer();
	};
}
hPrintfContext::~hPrintfContext()
{
	va_end(arg);
}

int hPrintfContext::getSize()
{
	va_list tmpList;
	va_copy(tmpList, arg);
	return myprint(0, format, tmpList);
}
int hPrintfContext::doFormat()
{
	va_list tmpList;
	va_copy(tmpList, arg);
	bufferPos = 0;
	int ret = myprint(this, format, tmpList);
	flushBuffer();
	return ret;
}

void hPrintfContext::flushBuffer()
{
	if (!buffer || bufferPos == 0)
		return;

	if (putblock)
		putblock(buffer, bufferPos, user);

	bufferPos = 0;
}

}

#ifdef STM32F4

int printf(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return sys.vlog(format, arg);
}

int vprintf(const char* format, va_list arg)
{
	return sys.vlog(format, arg);
}

#define THROW
int sprintf(char* buf, const char* format, ...) THROW
{
	va_list arg;
	va_start(arg, format);
	return vsnprintf(buf, 0xffffffff, format, arg);
}
int vsprintf(char* buf, const char* format, va_list arg) THROW
{
	return vsnprintf(buf, 0xffffffff, format, arg);
}
int snprintf(char* buf, size_t len, const char* format, ...) THROW
{
	va_list arg;
	va_start(arg, format);
	return vsnprintf(buf, len, format, arg);
}
int vsnprintf(char* buf, size_t len, const char* format, va_list arg) THROW
{
	hPrintfContext ctx(format, arg);

	size_t pos = 0;
	ctx.putchar = [&pos, &buf, len](char c, void* user)
	{
		if (len != 0xffffffff && pos == len - 1)
			return;
		buf[pos] = c;
		pos++;
	};

	ctx.doFormat();
	buf[pos] = 0;

	return pos;
}
#else
#define THROW throw()
#endif
