/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __ISTORAGE_H__
#define __ISTORAGE_H__

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
class IStorage
{
public:
	virtual bool clear() = 0;

	/**
	 * @brief Copies data into persistent storage.
	 * @param addr address in persistent storage
	 * @param data data to copy
	 * @param len length of data to copy
	 */
	virtual bool store(uint16_t addr, const void* data, int len) = 0;

	/**
	 * @brief Copies data from persistent storage.
	 * @param addr address in persistent storage
	 * @param data buffer in which data will be stored
	 * @param len length of data to copy
	 */
	virtual bool load(uint16_t addr, void* data, int len) = 0;

	/**
	 * @brief Copy object to persistent storage
	 */
	template<typename T>
	bool store(uint16_t addr, const T& obj)
	{
		return store(addr, &obj, sizeof(obj));
	}

	/**
	 * @brief Load object from persistent storage
	 */
	template<typename T>
	bool load(uint16_t addr, T& obj)
	{
		return load(addr, &obj, sizeof(obj));
	}
};

/**
 * @brief Holds addresses of variables used by official Robocore packages.
 */
namespace SystemPersistent
{
const uint16_t SYSTEM_SPACE_START = 7168;

const uint16_t SESSION_ID = SYSTEM_SPACE_START; //<! Session identifier used by hCloudClient
}

}

#endif
