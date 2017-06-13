/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYUSART_ISR_H
#define MYUSART_ISR_H

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint16_t UsartBufferLength;

int myUSART_write_isr(uint8_t nr, const void* data, uint32_t size, uint32_t timeout);
int myUSART_read_isr(uint8_t nr, void* data, uint32_t size, uint32_t timeout);
void myUSART_config_isr_tx(uint8_t nr);
void myUSART_config_isr_rx(uint8_t nr);
uint8_t myUSART_flushRXbuf_isr(uint8_t nr);
uint32_t myUSART_getReadBufCnt_isr(uint8_t nr);
int myUSART_waitForData(uint8_t nr, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* MYUSART_ISR_H */
