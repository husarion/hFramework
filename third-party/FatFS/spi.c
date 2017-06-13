#include "spi.h"
#include "mySPI.h"

void SPI_init(SPI_TypeDef* SPIx, unsigned int prescaler)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	SPI_InitTypeDef SPI_InitStruct;
//
//	// enable clock for used IO pins
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//
//	/* configure pins used by SPI3
//	 * PC10 = SCK
//	 * PC11 = MISO
//	 * PB5 = MOSI
//	 */
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//	// connect SPI1 pins to SPI alternate function
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
//
//	// enable peripheral clock
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
//
//	/* configure SPI1 in Mode 0
//	 * CPOL = 0 --> clock is low when idle
//	 * CPHA = 0 --> data is sampled at the first edge
//	 */
//	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
//	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
//	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
//	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
//	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
//	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
//	SPI_InitStruct.SPI_BaudRatePrescaler = prescaler; // SPI frequency is APB2 frequency / 4
//	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
//	SPI_Init(SPI3, &SPI_InitStruct);
//
//	SPI_Cmd(SPI3, ENABLE); // enable SPI1
}

void SPI_send_single(SPI_TypeDef* SPIx, unsigned char data)
{
	volatile unsigned char tmp;
	SPIx->DR = data; // write data to be transmitted to the SPI data register
	while (!(SPIx->SR & SPI_I2S_FLAG_TXE));  // wait until transmit complete
	while (!(SPIx->SR & SPI_I2S_FLAG_RXNE));  // wait until receive complete
	while (SPIx->SR & SPI_I2S_FLAG_BSY);  // wait until SPI is not busy anymore
	tmp = SPIx->DR; // return received data from SPI data register
	tmp; //to skip compiler error
}

unsigned char SPI_receive_single(SPI_TypeDef* SPIx)
{
	SPI3->DR = 0xFF; // write data to be transmitted to the SPI data register
	while (!(SPI3->SR & SPI_I2S_FLAG_TXE));  // wait until transmit complete
	while (!(SPI3->SR & SPI_I2S_FLAG_RXNE));  // wait until receive complete
	while (SPI3->SR & SPI_I2S_FLAG_BSY);  // wait until SPI is not busy anymore
	return SPI3->DR; // return received data from SPI data register
}

void SPI_send(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length)
{
//	while (length--)
//	{
//		SPI_send_single(SPIx, *data);
//		data++;
//	}
	mySPI_ReadWrite(3, data, 0, length);
}

void SPI_receive(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length)
{
//	while (length--)
//	{
//		*data = SPI_receive_single(SPIx);
//		data++;
//	}
	mySPI_ReadWrite(3, (uint8_t*)0xff, (uint8_t*)data, (uint32_t)length);
}

void SPI_transmit(SPI_TypeDef* SPIx, unsigned char* txbuf, unsigned char* rxbuf, unsigned int len)
{
	while (len--)
	{
		SPIx->DR = *txbuf; // write data to be transmitted to the SPI data register
		while (!(SPIx->SR & SPI_I2S_FLAG_TXE));  // wait until transmit complete
		while (!(SPIx->SR & SPI_I2S_FLAG_RXNE));  // wait until receive complete
		while (SPIx->SR & SPI_I2S_FLAG_BSY);  // wait until SPI is not busy anymore
		*rxbuf = SPIx->DR; // return received data from SPI data register
		txbuf++;
		rxbuf++;
	}
}
