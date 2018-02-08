/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "II2C.h"

namespace hFramework
{
	

bool II2C::read(uint8_t addr, uint8_t* dataTx, uint32_t lengthTx, uint8_t* dataRx, uint32_t lengthRx)
{
	return rw(addr, dataTx, lengthTx, dataRx, lengthRx, 0);
}

}
