/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYUSART_POLL_H
#define MYUSART_POLL_H

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

int myUSART_write_poll(uint8_t nr, const void* data, uint32_t size, uint32_t timeout);
int myUSART_read_poll(uint8_t nr, void* data, uint32_t size, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* MYUSART_POLL_H */
