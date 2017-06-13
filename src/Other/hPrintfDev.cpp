/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hPrintfDev.h"

#include "hPrintf.h"
#include "ycm.h"

namespace hFramework
{

hMutex printfMutex;

int hPrintfDev::printf(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	return vprintf(format, arg);
}

int hPrintfDev::vprintf(const char* format, va_list arg)
{
	hMutexGuard guard(printfMutex);

	hPrintfContext ctx(format, arg);

	ctx.user = this;
	ctx.putchar = [](char c, void* user)
	{
		((hPrintfDev*)user)->putchar(c);
	};

	int r = ctx.doFormat();
	return r;
}

int hPrintfDev::putchar(char c)
{
	int r = write(&c, 1, INFINITE);
	return r;
}

int hPrintfDev::getch()
{
	char c;
	int res = read(&c, 1, INFINITE);
	if (res <= 0)
		return res;
	return c;
}

}
