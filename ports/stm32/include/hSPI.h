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
#if BOARD(ROBOCORE) || BOARD(CORE2)
	hSPI_ID_EXT,
	hSPI_ID_SD,
#elif BOARD(CORE2MINI)
	hSPI_ID_SD,
#else
#  error no board
#endif
	hSPI_ID_INVALID,
};

enum class SPISpeed
{
	Speed42000,
	Speed21000,
	Speed10500,
	Speed5250,
	Speed2625,
	Speed1312,
	Speed656,
	Speed328,
};

class hSPI : public virtual ISPI
{
	friend class hExtClass;

public:
#if BOARD(ROBOCORE)
	hGPIO pinSck, pinMiso, pinMosi;
#elif BOARD(CORE2)
	hGPIO pinSck;
	hGPIO_adc pinMiso, pinMosi;
#elif BOARD(CORE2MINI)
	hGPIO pinSck, pinMiso, pinMosi;
#else
#  error no board
#endif

	hSPI(hSPI_ID no);

	void setSpeed(SPISpeed speed);
	bool write(void* data, uint32_t len) override;
	bool read(void* data, uint32_t len) override;
	bool rw(void* dataTx, void* dataRx, uint32_t len) override;

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
