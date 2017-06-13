/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSerial.h"

#include <debug.h>
// #include "hUtils.h"
// #include <hPrintf.h>

// #include <stdbool.h>
// #include <stdarg.h>

// #include <hSystem.h>
// #include <system.h>
// #include "debug.h"
// #include "ycm.h"
#include <cstdarg>
#include <cerrno>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <pty.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

int baudToDefine(int baudRate)
{
	switch (baudRate)
	{
	case 0: return B0;
	case 50: return B50;
	case 75: return B75;
	case 110: return B110;
	case 134: return B134;
	case 150: return B150;
	case 200: return B200;
	case 300: return B300;
	case 600: return B600;
	case 1200: return B1200;
	case 1800: return B1800;
	case 2400: return B2400;
	case 4800: return B4800;
	case 9600: return B9600;
	case 19200: return B19200;
	case 38400: return B38400;
	case 57600: return B57600;
	case 115200: return B115200;
	case 230400: return B230400;
	case 460800: return B460800;
	case 500000: return B500000;
	case 576000: return B576000;
	case 921600: return B921600;
	case 1000000: return B1000000;
	case 1152000: return B1152000;
	case 1500000: return B1500000;
	case 2000000: return B2000000;
	case 2500000: return B2500000;
	case 3000000: return B3000000;
	case 3500000: return B3500000;
	case 4000000: return B4000000;
	}
	return B0;
}

void tty_raw(int fd, int baudrate, Parity parity)
{
	//return;
	struct termios tty;
	memset(&tty, 0, sizeof tty);
	tcgetattr(fd, &tty);


	int parityflags = 0;
	switch (parity) {
	case Parity::None:
	default:
		parityflags = 0;
		break;
	case Parity::Odd:
		parityflags = PARENB |  PARODD;
		break;
	case Parity::Even:
		parityflags = PARENB;
		break;
	}
	tty.c_cflag = CBAUD | CS8 | parityflags | CREAD | CLOCAL;
	tty.c_iflag = IGNPAR;
	tty.c_oflag = 0;
	tty.c_lflag = 0;
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;

	tcflush(fd, TCIOFLUSH);
	cfsetospeed(&tty, baudToDefine(baudrate));
	cfsetispeed(&tty, baudToDefine(baudrate));

	tcsetattr(fd, TCSANOW, &tty);
}

namespace hFramework
{

// hSerialFD
class hSerialFDPimpl
{
public:
	struct termios old;
};

hSerialFD::hSerialFD()
	: write_fd(-1), read_fd(-1)
{
}
hSerialFD::hSerialFD(int write_fd, int read_fd)
	: write_fd(write_fd), read_fd(read_fd)
{
}
hSerialFD::~hSerialFD()
{
}

int hSerialFD::write(const void* data, int len, uint32_t timeout)
{
	return ::write(write_fd, data, len);
}
int hSerialFD::read(void* data, int len, uint32_t timeout)
{
	fd_set fdset;
	struct timeval timeout_val = {0, 0};
	timeout_val.tv_usec = timeout * 1000;

	struct timeval *timeout_ptr = &timeout_val;
	if (timeout == INFINITE)
		timeout_ptr = 0;

	FD_ZERO(&fdset);
	FD_SET(read_fd, &fdset);
	int r = select(read_fd + 1, &fdset, NULL, NULL, timeout_ptr);
	if (r == 0)
		return 0;

	int res = ::read(read_fd, data, len);
	if (res == -1 && errno == EAGAIN)
		return 0;
	return res;
}

void hSerialFD::flushRx()
{
	tcflush(read_fd, TCIOFLUSH);
}
uint32_t hSerialFD::available()
{
	int avail;
	ioctl(read_fd, FIONREAD, &avail);
	return avail;
}

void hSerialFD::init()
{
	// tty_raw(write_fd);
	// tty_raw(read_fd);
}

// hSerialFile
hSerialFile::hSerialFile(const char* path) : path(path)
{
}
hSerialFile::~hSerialFile()
{
}

bool hSerialFile::init(uint32_t baudrate, Parity parity)
{
	close(write_fd);
	if (read_fd != write_fd) close(read_fd);
	int serialfd = open(path, O_RDWR);

	if (serialfd > 0)
	{
		tty_raw(serialfd, 9600, Parity::None);
		tty_raw(serialfd, baudrate, parity);

		write_fd = serialfd;
		read_fd = serialfd;
		return true;
	}
	else
	{
		SYSLOG("Unable to open serial");
		return false;
	}
}

// hSerialPTY
hSerialPTY::hSerialPTY(const char* path) : path(path)
{
}
hSerialPTY::~hSerialPTY()
{
}

bool hSerialPTY::init()
{
	int master, slave;
	char tmpPath[64];
	pid_t pid = openpty(&master, &slave, tmpPath, 0, 0);
	if (pid < 0)
	{
		SYSLOG("Unable to allocate ptty");
		return false;
	}

	tty_raw(master, 500000, Parity::None);

	SYSLOG("created pty %s\r\n", tmpPath);
	chmod(tmpPath, 0666);

	SYSLOG("creating symlink %s\r\n", path);
	unlink(path);
	symlink(tmpPath, path);

	int flags = fcntl(master, F_GETFL, 0);
	flags = flags | O_NONBLOCK;

	fcntl(master, F_SETFL, flags);

	write_fd = master;
	read_fd = master;
}

}
