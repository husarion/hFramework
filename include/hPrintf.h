/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HPRINTF_H__
#define __HPRINTF_H__

#include <stddef.h>
#include <functional>

namespace hFramework
{

using putchar_cb_t = std::function<void(char c, void* user)>;
using putblock_cb_t = std::function<void(char* buffer, int len, void* user)>;

class hPrintfContext
{
public:
	putchar_cb_t putchar;
	putblock_cb_t putblock;
	const char* format;
	va_list arg;
	char* buffer;
	int bufferLen, bufferPos;
	int returnValue;
	void* user;

	hPrintfContext(const char* format, va_list inArg);
	hPrintfContext(const char* format, va_list inArg, char* buffer, int bufferLen);
	~hPrintfContext();

	int getSize();
	int doFormat();

private:
	void flushBuffer();
};

}

extern "C" int printf(const char* format, ...);
extern "C" int vprintf(const char* format, va_list arg);

extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" int vsprintf(char* buf, const char* format, va_list arg);
extern "C" int snprintf(char* buf, size_t len, const char* format, ...);
extern "C" int vsnprintf(char* buf, size_t len, const char* format, va_list arg);

#endif
