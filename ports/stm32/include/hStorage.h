/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSTORAGE_H__
#define __HSTORAGE_H__

#include <hSystem.h>
#include <hMutex.h>

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
class hStorage
{
public:
	hStorage();

	bool clear();

	/**
	 * @brief Copies data into persistent storage.
	 * @param addr address in persistent storage
	 * @param data data to copy
	 * @param len length of data to copy
	 */
	bool store(uint16_t addr, const void* data, int len);

	/**
	 * @brief Copies data from persistent storage.
	 * @param addr address in persistent storage
	 * @param data buffer in which data will be stored
	 * @param len length of data to copy
	 */
	bool load(uint16_t addr, void* data, int len);

	/**
	 * @brief Copy object to persistent storage
	 */
	template<typename T>
	bool store(uint16_t addr, const T& obj);

	/**
	 * @brief Load object from persistent storage
	 */
	template<typename T>
	bool load(uint16_t addr, T& obj);

private:
	void init();

	friend class hSystem;
};

template<typename T>
bool hStorage::store(uint16_t addr, const T& obj)
{
	const uint8_t* data = (uint8_t*)&obj;
	return store(addr, data, sizeof(obj));
}

template<typename T>
bool hStorage::load(uint16_t addr, T& obj)
{
	uint8_t* data = (uint8_t*)&obj;
	return load(addr, data, sizeof(obj));
}

/**
 * @brief Holds addresses of variables used by official Husarion packages.
 */
namespace SystemPersistent
{
const uint16_t SYSTEM_SPACE_START = 7168;

const uint16_t SESSION_ID = SYSTEM_SPACE_START; //<! Session identifier used by hCloudClient.
}

}

#endif
