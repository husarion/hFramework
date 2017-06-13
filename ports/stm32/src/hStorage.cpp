/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hStorage.h>
#define DEBUG_EEPROM
#define DEBUG_CATEGORY EEPROM
#include <debug.h>
#include <system.h>

extern "C"
{
#include "eeprom.h"
}

namespace hFramework
{

hMutex mutex;

static bool storeByte_internal(uint16_t addr, uint8_t val)
{
	uint16_t idx = addr / 2;

	uint16_t curVal = 0xffff;
	if (EE_ReadVariable(EE_GetVirtAddr(idx), &curVal) != 0)
		curVal = 0;

	LOG("data at %d is 0x%04x", idx, curVal);
	if (addr & 1)
		curVal = (curVal & 0x00ff) | (val << 8);
	else
		curVal = (curVal & 0xff00) | val;

	LOG("storing byte 0x%04x at %d (store: 0x%02x)", curVal, idx, val);
	if (EE_WriteVariable(EE_GetVirtAddr(idx), curVal) != 8)
		return false;

	return true;
}
static bool storeWord_internal(uint16_t addr, uint16_t val)
{
	uint16_t idx = addr / 2;

	ASSERT((addr & 1) == 0); // word store is available only for word-alignment

	LOG("storing word 0x%04x at %d", val, idx);
	if (EE_WriteVariable(EE_GetVirtAddr(idx), val) != 8)
		return false;

	return true;
}
static bool loadByte_internal(uint16_t addr, uint8_t& value)
{
	uint16_t idx = addr / 2;

	uint16_t curVal;
	if (EE_ReadVariable(EE_GetVirtAddr(idx), &curVal) != 0)
		return false;

	if (addr & 1)
		value = (curVal & 0xff00) >> 8;
	else
		value = curVal & 0x00ff;

	LOG("loading byte 0x%04x at %d (load: 0x%02x)", curVal, addr, value);

	return true;
}

hStorage::hStorage()
{
}
void hStorage::init()
{
	LOG("hStorage::init");

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	                FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

	EE_Init();

	FLASH_Lock();
}

bool hStorage::clear()
{
	hMutexGuard guard(mutex);

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	                FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

	int res = EE_Format();

	FLASH_Lock();

	return res == 8;
}

bool hStorage::store(uint16_t addr, const void* data, int len)
{
	hMutexGuard guard(mutex);

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	                FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

	const uint8_t *_data = (uint8_t*)data;

	// write word alignment
	if (addr & 1)
	{
		uint8_t _data8 = *(uint8_t*)_data;
		bool res = storeByte_internal(addr, _data8);
		if (!res)
		{
			FLASH_Lock();
			return false;
		}
		addr += 1;
		_data += 1;
		len -= 1;
	}

	// write bytes in series of two
	while (len >= 2)
	{
		uint16_t _data16 = *(uint16_t*)_data;
		bool res = storeWord_internal(addr, _data16);
		if (!res)
		{
			FLASH_Lock();
			return false;
		}
		addr += 2;
		_data += 2;
		len -= 2;
	}

	// write remaining byte
	if (len == 1)
	{
		uint8_t _data8 = *(uint8_t*)_data;
		bool res = storeByte_internal(addr, _data8);
		if (!res)
		{
			FLASH_Lock();
			return false;
		}
	}

	FLASH_Lock();
	return true;
}
bool hStorage::load(uint16_t addr, void* data, int len)
{
	hMutexGuard guard(mutex);

	uint8_t *_data = (uint8_t*)data;
	for (int i = 0; i < len; i++)
		if (!loadByte_internal(addr + i, _data[i]))
			return false;

	return true;
}

}
