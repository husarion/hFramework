/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __MYPRINTF_H__
#define __MYPRINTF_H__

#include "hPrintf.h"

using namespace hFramework;

int myprint(hPrintfContext* ctx, const char *format, va_list args);

#endif
