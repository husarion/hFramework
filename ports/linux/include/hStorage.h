/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSTORAGE_H__
#define __HSTORAGE_H__

#include <IStorage.h>

namespace hFramework
{

/**
 * @brief Provides local persistent storage.
 *
 * There are 8 KiB available for storage - the last 1 KiB is reserved for hFramework internal configuration.

 * All data is stored in virtual 16-bit address space. Addresses from 0 to 7168 bytes are available for user,
 * others are reserved for internal hFramework use.
 *
 * Persistent storage is implemented using EEPROM emulation, which works on two dedicated flash pages.
 */
class hStorage : public IStorage
{
public:
	hStorage();

	bool clear();
	bool store(uint16_t addr, const void* data, int len);
	bool load(uint16_t addr, void* data, int len);
	using IStorage::store;
	using IStorage::load;

private:
	void init();

	friend class hSystem;
};

}

#endif
