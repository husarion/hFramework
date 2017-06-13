/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSPI.h"
#include "hGPIO.h"
#include "mySPI.h"

#include "hSystem.h"
#include "hUtils.h"
#include "debug.h"
#include "myGPIO.h"

namespace hFramework
{

struct
{
	uint8_t spiNum;
	uint8_t pinSck, pinMiso, pinMosi;
} const hSPIDefs[] =
{
#if BOARD(ROBOCORE)
	{ 2, MYGPIO_EXT_SPI_SCK, MYGPIO_EXT_SPI_MISO, MYGPIO_EXT_SPI_MOSI },
	{ 3, MYGPIO_SD_CLK,      MYGPIO_SD_MISO,      MYGPIO_SD_MOSI },
#elif BOARD(CORE2)
	{ 2, MYGPIO_EXT_SPI_SCK, MYGPIO_EXT_SPI_MISO, MYGPIO_EXT_SPI_MOSI },
	{ 3, MYGPIO_SD_CLK,      MYGPIO_SD_MISO,      MYGPIO_SD_MOSI },
#elif BOARD(CORE2MINI)
	{ 3, MYGPIO_SD_CLK,      MYGPIO_SD_MISO,      MYGPIO_SD_MOSI },
#else
#  error no board
#endif
};


hSPI::hSPI(hSPI_ID no)
	: pinSck(hSPIDefs[no].pinSck),
	  pinMiso(hSPIDefs[no].pinMiso),
	  pinMosi(hSPIDefs[no].pinMosi),
	  spiNum(hSPIDefs[no].spiNum)
{
}

#define SPI_BaudRatePrescaler_2   ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4   ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8   ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16  ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32  ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64  ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128 ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256 ((uint16_t)0x0038)

void hSPI::init()
{
	mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_256);
	selectSPI();
}
void hSPI::deinit()
{
}
void hSPI::setSpeed(SPISpeed speed)
{
	switch (speed)
	{
	default:
	case SPISpeed::Speed42000: mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_2); break;
	case SPISpeed::Speed21000: mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_4); break;
	case SPISpeed::Speed10500: mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_8); break;
	case SPISpeed::Speed5250:  mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_16); break;
	case SPISpeed::Speed2625:  mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_32); break;
	case SPISpeed::Speed1312:  mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_64); break;
	case SPISpeed::Speed656:   mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_128); break;
	case SPISpeed::Speed328:   mySPI_Config(spiNum, 0, SPI_BaudRatePrescaler_256); break;
	}
}

bool hSPI::write(void* data, uint32_t len)
{
	mySPI_ReadWrite(spiNum, (uint8_t*)data, 0, len);
	return true;
}
bool hSPI::read(void* data, uint32_t len)
{
	mySPI_ReadWrite(spiNum, (uint8_t*)0xff, (uint8_t*)data, len);
	return true;
}
bool hSPI::rw(void* dataTx, void* dataRx, uint32_t len)
{
	mySPI_ReadWrite(spiNum, (uint8_t*)dataTx, (uint8_t*)dataRx, len);
	return true;
}

void hSPI::selectGPIO()
{
	mySPI_releaseGPIO(spiNum);
	pinSck.init();
	pinMiso.init();
	pinMosi.init();
}
void hSPI::selectSPI()
{
	mySPI_acquireGPIO(spiNum);
}

}
