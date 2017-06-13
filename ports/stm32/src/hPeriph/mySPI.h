/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYSPI_H_
#define MYSPI_H_

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void mySPI_Config(uint8_t spiNum, uint8_t mode, uint16_t BaudRatePrescaler);
void mySPI_ReadWrite(uint8_t spiNum, uint8_t* tx_buf, uint8_t* rx_buf, uint32_t size);
void mySPI_DMA_send_data(uint8_t spiNum, uint8_t* tx_buf, uint8_t* rx_buf, uint32_t size);

void mySPI_acquireGPIO(uint8_t spiNum);
void mySPI_releaseGPIO(uint8_t spiNum);

#ifdef __cplusplus
}
#endif

#endif /* MYSPI_H_ */
