/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>

#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mutex>
#include <unistd.h>
#include <sys/types.h>

namespace hFramework
{

std::mutex storage_mutex;
char* memory;

const int size = 1024 * 8;

hStorage::hStorage()
{
}

void hStorage::init()
{
	int fd = open("hStorage.eeprom", O_RDWR | O_CREAT, 0777);
	if (fd < 0) {
		perror("hStorage open");
		exit(1);
	}
	int ret = ftruncate(fd, size);
	if (ret < 0) {
		perror("truncate");
		exit(1);
	}
	void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == nullptr) {
		perror("mmap");
		exit(1);
	}
	memory = (char*) mem;
}

bool hStorage::clear()
{
	std::unique_lock<std::mutex> lock(storage_mutex);

	memset(memory, 0, size);
	return true;
}

bool hStorage::store(uint16_t addr, const void* data, int len)
{
	std::unique_lock<std::mutex> lock(storage_mutex);

	// assert(addr >= 0 && len >= 0 && addr + len < size);
	memcpy(memory + addr, data, len);
	msync(memory, size, 0);
	return true;
}

bool hStorage::load(uint16_t addr, void* data, int len)
{
	std::unique_lock<std::mutex> lock(storage_mutex);

	// assert(addr >= 0 && len >= 0 && addr + len < size);
	memcpy(data, memory + addr, len);
	return true;
}

}
