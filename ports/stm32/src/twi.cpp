#include <hFramework.h>
#include <twi.h>

#ifndef TWI
#define TWI hSens1.i2c
#endif

void twi_init(void){
TWI.setDataRate(100000);
}

void twi_disable(void){}

uint8_t I2C_ADDR = TWI_FREQ;
void twi_setAddress(uint8_t adress){
  I2C_ADDR = adress;
}

void twi_setFrequency(uint32_t freq){
  TWI.setDataRate(freq);
}

uint8_t twi_readFrom(uint8_t adress, uint8_t* data, uint8_t len, uint8_t stop){
  return TWI.read(adress, data, len);
}

uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop){
   return TWI.write (address, *data, length);
}

//uint8_t twi_transmit(const uint8_t*, uint8_t)
  
void twi_attachSlaveRxEvent( void (*)(uint8_t*, int) ){}
void twi_attachSlaveTxEvent( void (*)(void) );

void twi_reply(uint8_t){}//TODO:

void twi_stop(void){}

void twi_releaseBus(void){}

