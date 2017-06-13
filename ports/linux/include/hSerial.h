/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSERIAL_H__
#define __HSERIAL_H__

#include <ISerial.h>

namespace hFramework
{

class hSerialFD : public virtual ISerial
{
public:
	hSerialFD();
	hSerialFD(int write_fd, int read_fd);
	virtual ~hSerialFD();

	void init();

	void setBaudrate(uint32_t baudrate) {}

	int write(const void* data, int len, uint32_t timeout = INFINITE) override;
	int read(void* data, int len, uint32_t timeout = INFINITE) override;

	void flushRx() override;
	uint32_t available() override;

protected:
	int write_fd = -1, read_fd = -1;

	hSerialFD(const hSerialFD&) = delete;
};

class hSerialFile : public hSerialFD
{
public:
	hSerialFile(const char* path);
	virtual ~hSerialFile();

	bool init(uint32_t baudrate = 230400, Parity parity = Parity::None);

protected:
	const char* path;
};

class hSerialPTY : public hSerialFD
{
public:
	hSerialPTY(const char* path);
	virtual ~hSerialPTY();

	bool init();
private:
	const char* path;
};

class hBoardSerial : public hSerialFile {
	int num;
public:
	hBoardSerial(int num): hSerialFile(nullptr), num(num) {}

	bool init(uint32_t baudrate = 230400, Parity parity = Parity::None);
};

extern hBoardSerial Serial1;

}


#endif
