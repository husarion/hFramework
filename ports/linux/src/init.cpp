/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>

#include <pty.h>
#include <string.h>
#include <signal.h>

void hMain();

namespace hFramework
{
hSerialFD Stdio(0, 1);

void portInit()
{
	Stdio.init();
}

struct termios fd0, fd1, fd2;
void restoreio()
{
	::printf("atexit\r\n");
	tcsetattr(0, TCSANOW, &fd0);
	tcsetattr(1, TCSANOW, &fd1);
	tcsetattr(2, TCSANOW, &fd2);
}
}

using namespace hFramework;

int main(void)
{
	signal(SIGPIPE, SIG_IGN);
	setvbuf(stdout, NULL, _IONBF, 0);
#ifdef RAW_TERMINAL
	// emulate serial console
	memset(&fd0, 0, sizeof(fd0));
	memset(&fd1, 0, sizeof(fd1));
	memset(&fd2, 0, sizeof(fd2));
	tcgetattr(0, &fd0);
	tcgetattr(1, &fd1);
	tcgetattr(2, &fd2);

	atexit(restoreio);
#endif

	portInit();
	hMain();

	return 0;
}
