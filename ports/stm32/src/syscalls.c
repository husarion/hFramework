#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "system.h"

#undef errno
extern unsigned int errno;
extern unsigned int _end;
extern unsigned int __HeapLimit;

int sys_log_data_crnl(const char* buf, int len);

__attribute__((used))
caddr_t _sbrk(int incr)
{
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;
	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}

	prev_heap = heap;
	heap += incr;

	if ((int)(heap) + 0x100 > 0x20020000) {
		sys_fault_log("heap memory exhausted\r\n");
		sys_fault_handler();
	}

	return (caddr_t) prev_heap;
}

__attribute__((used))
int link(char *old, char *new)
{
	return -1;
}

__attribute__((used))
int _close(int file)
{
	return -1;
}

__attribute__((used))
int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

__attribute__((used))
int _isatty(int file)
{
	return 1;
}

__attribute__((used))
int _lseek(int file, int ptr, int dir)
{
	return 0;
}
__attribute__((used))
int _read(int file, char *ptr, int len)
{
	if (file == 0)
	{
		return sys_read_log(ptr, 1);
	}
	return 0;
}
__attribute__((used))
int _write(int file, char *ptr, int len)
{
	if (file == 1)
	{
		return sys_log_data_crnl(ptr, len);
	}
	return 0;
}

__attribute__((used))
void abort(void)
{
	const char msg[] = "abort called\n";
	sys_log_data_crnl(msg, sizeof(msg));
	sys_fault_handler();
	__builtin_unreachable();
}

__attribute__((used))
void _exit(int n)
{
	const char msg[] = "exit called\n";
	sys_log_data_crnl(msg, sizeof(msg));
	sys_fault_handler();
}

int sys_log_data_crnl(const char* buf, int len)
{
	int start = 0;
	for (int i = 0; i <= len; i++)
	{
		if (i == len)
		{
			int plen = i - start;
			if (plen > 0)
				sys_write_log(buf + start, plen);
		}
		else if (buf[i] == '\n')
		{
			int plen = i - start;
			sys_write_log(buf + start, plen);
			sys_write_log("\n\r", 2);
			start += plen + 1;
		}
	}
	return len;
}
