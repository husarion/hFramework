/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hGPIO.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include <stropts.h>
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fstream>
#include <string>

#include <condition_variable>

const int GPIO_NUM = 3;
struct pollfd fds[GPIO_NUM];

namespace hFramework
{

void writeFile(std::string path, std::string value) {
	std::ofstream f (path);
	f << value;
}

std::string readFile(std::string path) {
	std::ifstream f (path);
	std::string value;
	f >> value;
	return value;
}

void initBoard();

void hGPIO::init()
{
	initBoard();
	writeFile("/sys/class/gpio/export", std::to_string(nr));

	mkdir("/run/gpio", 0755);
	std::string valueFile = "/sys/class/gpio/gpio" + std::to_string(nr) + "/value";
	std::string runFile = "/run/gpio/" + std::to_string(id);
	symlink(valueFile.c_str(), runFile.c_str());
}

void hGPIO::setOut()
{
	writeFile("/sys/class/gpio/gpio" + std::to_string(nr) + "/direction", "out");
}
void hGPIO::setIn()
{
	writeFile("/sys/class/gpio/gpio" + std::to_string(nr) + "/direction", "in");
}

bool hGPIO::read()
{
	return readFile("/sys/class/gpio/gpio" + std::to_string(nr) + "/value") == "1";
}

void hGPIO::write(bool value)
{
	writeFile("/sys/class/gpio/gpio" + std::to_string(nr) + "/value", value ? "1" : "0");
}
}
