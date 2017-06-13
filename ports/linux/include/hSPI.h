/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSPI_H__
#define __HSPI_H__

#include <stdint.h>

#include <hTypes.h>
#include <hGPIO.h>
#include <ISPI.h>

namespace hFramework
{

enum hSPI_ID
{
	hSPI_ID_EXT,
	hSPI_ID_SD,
	hSPI_ID_INVALID,
};

class hSPI : public virtual ISPI
{
	friend class hExt;

public:
	hGPIO pinSck, pinMiso, pinMosi;

	hSPI(hSPI_ID no);

	void setPrescaler(int prescaler);
	bool write(uint8_t* data, uint32_t len);
	bool read(uint8_t* data, uint32_t len);
	bool rw(uint8_t* dataTx, uint8_t* dataRx, uint32_t len);

	void selectGPIO();
	void selectSPI();

private:
	int spiNum;

	hSPI(const hSPI&) = delete;

	void init();
	void deinit();
};

}

#endif
