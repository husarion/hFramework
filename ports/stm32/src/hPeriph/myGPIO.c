/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myGPIO.h"
#include "stm32f4xx.h"

#include "myUSART.h"
#include "hTypes.h"
#include "system.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

typedef struct
{
	uint16_t pin;
	uint16_t pinpos;
	GPIO_TypeDef* port;
	uint32_t gpio_clock;
	int8_t adc_nr;

	uint8_t EXTI_PortSourceGPIOx;
	uint8_t EXTI_PinSourcex;
	uint32_t EXTI_Linex;
	uint8_t EXTIy_IRQn;
} GPIO_desc_t;

#define DEFINE_PORT____(name,nr,port,pin,adc) { GPIO_Pin_ ## pin, pin, GPIO ## port, RCC_AHB1Periph_GPIO ## port, adc, 0, 0, 0, 0 },
#define DEFINE_PORT_IRQ(name,nr,port,pin,adc,irq) { GPIO_Pin_ ## pin, pin, GPIO ## port, RCC_AHB1Periph_GPIO ## port, adc, \
	EXTI_PortSourceGPIO ## port, EXTI_PinSource ## pin, EXTI_Line ## pin, irq },

const GPIO_desc_t gpio[] =
{
#include "myGPIO_include.h"
};

void myGPIO_config(uint8_t pinNr)
{
	RCC_AHB1PeriphClockCmd(gpio[pinNr].gpio_clock, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = gpio[pinNr].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio[pinNr].port, &GPIO_InitStructure);
}
void myGPIO_config_af(uint8_t pinNr, int af)
{
	RCC_AHB1PeriphClockCmd(gpio[pinNr].gpio_clock, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = gpio[pinNr].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio[pinNr].port, &GPIO_InitStructure);

	GPIO_PinAFConfig(gpio[pinNr].port, gpio[pinNr].pinpos, af);
}

void myGPIO_EXTIconfig(uint8_t pinNr, extiMode_t extiMode)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if (0 == gpio[pinNr].EXTI_Linex)
	{
		return;
	}

	/* Enable GPIOA clock */
	RCC_AHB1PeriphClockCmd(gpio[pinNr].gpio_clock, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // TODO: remove this
	GPIO_InitStructure.GPIO_Pin = gpio[pinNr].pin;
	GPIO_Init(gpio[pinNr].port, &GPIO_InitStructure);

	/* Connect EXTI Line10 to PF10 pin */
	SYSCFG_EXTILineConfig(gpio[pinNr].EXTI_PortSourceGPIOx, gpio[pinNr].EXTI_PinSourcex);

	/* Configure EXTI Line10 */
	EXTI_InitStructure.EXTI_Line = gpio[pinNr].EXTI_Linex;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	if (EXTI_RISING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	}
	if (EXTI_FALLING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	}
	if (EXTI_RISING_FALLING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	}
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = gpio[pinNr].EXTIy_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(gpio[pinNr].EXTIy_IRQn);
}

void myGPIO_EXTI_off(uint8_t pinNr)
{
	//NVIC_DisableIRQ(gpio[pinNr].EXTIy_IRQn);
	SYSCFG_EXTILineConfig(0x0f, gpio[pinNr].EXTI_PinSourcex);
}

void myGPIO_EXTI_on(uint8_t pinNr)
{
	//	NVIC_InitTypeDef   NVIC_InitStructure;
	//
	//	NVIC_InitStructure.NVIC_IRQChannel = gpio[pinNr].EXTIy_IRQn;
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2;
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//	NVIC_Init(&NVIC_InitStructure);

	//NVIC_EnableIRQ(gpio[pinNr].EXTIy_IRQn);
	SYSCFG_EXTILineConfig(gpio[pinNr].EXTI_PortSourceGPIOx, gpio[pinNr].EXTI_PinSourcex);
}

void myGPIO_EXTI_setEdge(uint8_t pinNr, extiMode_t extiMode)
{
	// TODO: what if interrupt arrives now?
	EXTI_InitTypeDef   EXTI_InitStructure;

	if (0 == gpio[pinNr].EXTI_Linex)
	{
		return;
	}

	EXTI_InitStructure.EXTI_Line = gpio[pinNr].EXTI_Linex;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	if (EXTI_RISING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	}
	if (EXTI_FALLING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	}
	if (EXTI_RISING_FALLING == extiMode)
	{
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	}
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	portENTER_CRITICAL();
	EXTI_Init(&EXTI_InitStructure);
	portEXIT_CRITICAL();
}

void myGPIO_dir_out(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->MODER &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
	GPIOx->MODER |= (((uint32_t)GPIO_Mode_OUT) << (pinpos * 2));
}

void myGPIO_outType_pp(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->OTYPER &= ~((GPIO_OTYPER_OT_0) << pinpos);
	GPIOx->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << (pinpos));
}

void myGPIO_outType_od(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->OTYPER &= ~((GPIO_OTYPER_OT_0) << pinpos);
	GPIOx->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_OD) << (pinpos));
}

void myGPIO_dir_in(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->MODER &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
	GPIOx->MODER |= (((uint32_t)GPIO_Mode_IN) << (pinpos * 2));
}

void myGPIO_pupd_none(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
	GPIOx->PUPDR |= (((uint32_t)GPIO_PuPd_NOPULL) << (pinpos * 2));
}

void myGPIO_pupd_pu(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
	GPIOx->PUPDR |= (((uint32_t)GPIO_PuPd_UP) << (pinpos * 2));
}

void myGPIO_pupd_pd(uint8_t pinNr)
{
	GPIO_TypeDef* GPIOx = gpio[pinNr].port;
	uint16_t pinpos = gpio[pinNr].pinpos;
	GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
	GPIOx->PUPDR |= (((uint32_t)GPIO_PuPd_DOWN) << (pinpos * 2));
}

bool myGPIO_read(uint8_t pinNr)
{
	return (gpio[pinNr].port->IDR & gpio[pinNr].pin) != 0;
}
void myGPIO_write(uint8_t pinNr, bool value)
{
	if (value)
		gpio[pinNr].port->BSRRL = gpio[pinNr].pin;
	else
		gpio[pinNr].port->BSRRH = gpio[pinNr].pin;
}
void myGPIO_toggle(uint8_t pinNr)
{
	myGPIO_write(pinNr, !(gpio[pinNr].port->ODR & gpio[pinNr].pin));
}

void myGPIO_setAnalogIn(uint8_t pinNr, pullUpDown_t pud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = gpio[pinNr].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	switch (pud)
	{
	case PU_ENABLE:
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;
	case PD_ENABLE:
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;
	case PUPD_DISABLE:
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio[pinNr].port, &GPIO_InitStructure);
}

int myGPIO_getAdcNr(uint8_t pinNr)
{
	return gpio[pinNr].adc_nr;
}
