/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __MYGPIO_H__
#define __MYGPIO_H__

#include "hTypes.h"
#include <stdbool.h>

#define DEFINE_PORT____(name,nr,port,pin,adc) name = nr,
#define DEFINE_PORT_IRQ(name,nr,port,pin,adc,irq) name = nr,

enum
{
#include "myGPIO_include.h"
};

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	PU_ENABLE,
	PD_ENABLE,
	PUPD_DISABLE
} pullUpDown_t;

typedef enum
{
	EXTI_RISING = 0,
	EXTI_FALLING,
	EXTI_RISING_FALLING
} extiMode_t;

void myGPIO_config(uint8_t pinNr);
void myGPIO_config_af(uint8_t pinNr, int af);
void myGPIO_EXTI_off(uint8_t pinNr);
void myGPIO_EXTI_on(uint8_t pinNr);

void myGPIO_EXTIconfig(uint8_t pinNr, extiMode_t extiMode);
void myGPIO_EXTI_setEdge(uint8_t pinNr, extiMode_t extiMode);

bool myGPIO_read(uint8_t pinNr);
void myGPIO_dir_out(uint8_t pinNr);
void myGPIO_outType_pp(uint8_t pinNr);
void myGPIO_outType_od(uint8_t pinNr);
void myGPIO_dir_in(uint8_t pinNr);
void myGPIO_pupd_none(uint8_t pinNr);
void myGPIO_pupd_pu(uint8_t pinNr);
void myGPIO_pupd_pd(uint8_t pinNr);
void myGPIO_write(uint8_t pinNr, bool value);
void myGPIO_toggle(uint8_t pinNr);

void myGPIO_setAnalogIn(uint8_t pinNr, pullUpDown_t pud);

int myGPIO_getAdcNr(uint8_t pinNr);

#ifdef __cplusplus
}
#endif

#endif /* __MYGPIO_H__ */
