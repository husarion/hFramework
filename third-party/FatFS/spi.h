#ifndef _SPI_H_
#define _SPI_H_

#include <stm32f4xx.h>

#ifdef __cplusplus
extern "C" {
#endif

void SPI_init(SPI_TypeDef* SPIx, unsigned int prescaler);
void SPI_send_single(SPI_TypeDef* SPIx, unsigned char data);
unsigned char SPI_receive_single(SPI_TypeDef* SPIx);
void SPI_send(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length);
void SPI_receive(SPI_TypeDef* SPIx, unsigned char* data, unsigned int length);
void SPI_transmit(SPI_TypeDef* SPIx, unsigned char* txbuf, unsigned char* rxbuf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // _SPI_H_
