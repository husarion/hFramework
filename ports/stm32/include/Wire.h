/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
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

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/
#ifndef TwoWire_h
#define TwoWire_h

#define BUFFER_LENGTH 16

#include <stdint.h>
#include <stdlib.h>
//#include <string.h>
#include <twi.h>
//#include <Stream.h>

class TwoWire
{
	// Initialize Class Variables //////////////////////////////////////////////////

	uint8_t rxBuffer[BUFFER_LENGTH];
	uint8_t rxBufferIndex = 0;
	uint8_t rxBufferLength = 0;

	uint8_t txAddress = 0;
	uint8_t txBuffer[BUFFER_LENGTH];
	uint8_t txBufferIndex = 0;
	uint8_t txBufferLength = 0;

	uint8_t transmitting = 0;
	void (*user_onRequest)(void);
	void (*user_onReceive)(int);

	// Constructors ////////////////////////////////////////////////////////////////
public:
	TwoWire();

	// Public Methods //////////////////////////////////////////////////////////////
	void begin(void);
	void begin(uint8_t address);
	void begin(int address);
	void end(void);
	void setClock(uint32_t clock);
	uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
	uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
	uint8_t requestFrom(uint8_t address, uint8_t quantity);
	uint8_t requestFrom(int address, int quantity);
	uint8_t requestFrom(int address, int quantity, int sendStop);
	void beginTransmission(uint8_t address);
	void beginTransmission(int address);
	uint8_t endTransmission(uint8_t sendStop);
	uint8_t endTransmission(void);
	size_t write(uint8_t data);
	size_t write(uint8_t *data, size_t quantity);
	int available(void);
	int read(void);
	int peek(void);
	void flush(void);
	void onReceiveService(uint8_t* inBytes, int numBytes);
	void onRequestService(void);
	void onReceive(void (*function)(int));
	void onRequest(void (*function)(void));
};
// Preinstantiate Objects //////////////////////////////////////////////////////

extern TwoWire Wire;

#endif