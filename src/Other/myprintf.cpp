/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdarg.h>
#include <stdint.h>

#include "myprintf.h"

using namespace hFramework;

static void printchar(hPrintfContext* ctx, int c)
{
	if (ctx)
		ctx->putchar(c, ctx->user);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(hPrintfContext* ctx, const char *string, int width, int pad)
{
	int pc = 0, padchar = ' ';

	if (width > 0)
	{
		int len = 0;
		const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT))
	{
		for (; width > 0; --width)
		{
			printchar(ctx, padchar);
			++pc;
		}
	}
	for (; *string ; ++string)
	{
		printchar(ctx, *string);
		++pc;
	}
	for (; width > 0; --width) 
	{
		printchar(ctx, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(hPrintfContext* ctx, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	char *s;
	int t, neg = 0, pc = 0;
	unsigned int u = i;

	if (i == 0)
	{
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(ctx, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0)
	{
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while (u)
	{
		t = u % b;
		if (t >= 10)
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg)
	{
		if (width && (pad & PAD_ZERO))
		{
			printchar(ctx, '-');
			++pc;
			--width;
		}
		else
		{
			*--s = '-';
		}
	}

	return pc + prints(ctx, s, width, pad);
}

static int printff(hPrintfContext* ctx, double val, int width, int pad)
{
	int pc = 0;

	if (width > 9)
		width = 9;

	if (val < 0)
	{
		printchar(ctx, '-');
		pc++;
		val = -val;
	}
	else if (pad == PAD_RIGHT)
	{
		printchar(ctx, ' ');
		pc++;
	}

	int mult = 1000000;
	int multSize = 6;
	if (width)
	{
		int tmp = width;
		mult = 10;
		while (--tmp)
			mult *= 10;
		multSize = width;
	}

	int32_t d = val;
	int32_t f = (int32_t)((val - d) * mult * 10);

	if (f % 10 >= 5)
	{
		f = f / 10;
		f += 1;
		if (f == mult)
		{
			f = 0;
			d += 1;
		}
	}
	else
	{
		f = f / 10;
	}

	pc += printi(ctx, d, 10, 0, 0, 0, 'a');
	printchar(ctx, '.');
	pc++;
	pc += printi(ctx, f, 10, 0, multSize, PAD_ZERO, 'a');

	return pc;
}

int myprint(hPrintfContext* ctx, const char *format, va_list args)
{
	int width, pad;
	int pc = 0;
	char scr[2];

	for (; *format != 0; ++format)
	{
		if (*format == '%')
		{
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-')
			{
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0')
			{
				++format;
				pad |= PAD_ZERO;
			}
			for (; *format >= '0' && *format <= '9'; ++format)
			{
				width *= 10;
				width += *format - '0';
			}
			if (*format == 's')
			{
				char *s = (char*)va_arg(args, char*);
				pc += prints(ctx, s ? s : "(null)", width, pad);
				continue;
			}
			if (*format == 'd')
			{
				pc += printi(ctx, va_arg(args, int), 10, 1, width, pad, 'a');
				continue;
			}
			if (*format == 'x' || *format == 'p')
			{
				pc += printi(ctx, va_arg(args, int), 16, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'X')
			{
				pc += printi(ctx, va_arg(args, int), 16, 0, width, pad, 'A');
				continue;
			}
			if (*format == 'u')
			{
				pc += printi(ctx, va_arg(args, int), 10, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'f')
			{
				pc += printff(ctx, va_arg(args, double), width, pad);
				continue;
			}
			if (*format == 'c')
			{
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg(args, int);
				scr[1] = '\0';
				pc += prints(ctx, scr, width, pad);
				continue;
			}
		}
		else
		{
out:
			printchar(ctx, *format);
			++pc;
		}
	}
	va_end(args);
	return pc;
}
