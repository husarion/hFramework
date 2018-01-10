/*
  twi.h - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <stdio.h>
#include <Arduino.h>

#ifndef twi_h
#define twi_h

//#define I2C hExt.i2c
//#define I2C1 hSens1.getSoftwareI2C()
//#define I2C2 hSens2.getSoftwareI2C()

#ifndef TWI
#define TWI hExt.i2c
#endif

const int TWI_FREQ = 100000;

void twi_init(void);
void twi_disable(void);
void twi_setAddress(uint8_t adress);
void twi_setFrequency(uint32_t freq);
uint8_t twi_readFrom(uint8_t adress, uint8_t* data, uint8_t length, uint8_t sendStop);
uint8_t twi_writeTo(uint8_t adress, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop);
uint8_t twi_transmit(uint8_t* data, uint8_t length);
void twi_attachSlaveRxEvent(void (*)(uint8_t*, int));
void twi_attachSlaveTxEvent(void (*)(void));
void twi_reply(uint8_t data);
void twi_stop(void);
void twi_releaseBus(void);

#endif
