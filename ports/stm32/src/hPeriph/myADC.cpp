/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "myUSART.h"
#include "myADC.h"
#include "myGPIO.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "debug.h"

const int ADC_CNT = 3;

// Constatnt info for each ADC
struct adc_desc_t
{
	uint32_t RCC_APB2Periph_ADCx;
	ADC_TypeDef* ADCx;
} const adc_desc[ADC_CNT] =
{
	{ RCC_APB2Periph_ADC1, ADC1 },
	{ RCC_APB2Periph_ADC2, ADC2 },
	{ RCC_APB2Periph_ADC3, ADC3 },
};

// Runtime info for each ADC
struct adc_data_t
{
	xSemaphoreHandle semHandle;
	xSemaphoreHandle mutexHandle;
} adc_data[ADC_CNT];

// Constatnt info for each pin associated with ADC
#define DEFINE_ADC(adc,channel) { adc-1, channel },
struct adc_pin_desc_t
{
	uint8_t adc, channel;
} const adc_pins_desc[] =
{
#include "myGPIO_include.h"
};

void myADC_init()
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;

	for (int i = 0; i < ADC_CNT; i++)
	{
		const adc_desc_t& adcDesc = adc_desc[i];
		adc_data_t& adc = adc_data[i];

		adc.semHandle = xSemaphoreCreateBinary();
		ASSERT(adc.semHandle != 0);
		xSemaphoreTake(adc.semHandle, 0);

		adc.mutexHandle = xSemaphoreCreateMutex();
		ASSERT(adc.mutexHandle != 0);
		xSemaphoreGive(adc.mutexHandle);

		RCC_APB2PeriphClockCmd(adcDesc.RCC_APB2Periph_ADCx, ENABLE);

		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
		ADC_CommonInit(&ADC_CommonInitStructure);

		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC_Init(adcDesc.ADCx, &ADC_InitStructure);
		ADC_Cmd(adcDesc.ADCx, ENABLE);

		NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		ADC_ITConfig(adcDesc.ADCx, ADC_IT_EOC, ENABLE);
	}
}

void myADC_config(uint8_t pinNr)
{
}

uint16_t myADC_readPin(uint8_t pinNr)
{
	int8_t adcEntryNr = myGPIO_getAdcNr(pinNr);
	ASSERT(adcEntryNr != -1);

	adc_pin_desc_t pin = adc_pins_desc[adcEntryNr];

	const adc_desc_t& adcDesc = adc_desc[pin.adc];
	adc_data_t& adc = adc_data[pin.adc];
	int channel = pin.channel;

	if (xSemaphoreTake(adc.mutexHandle, 10))
	{
		myGPIO_setAnalogIn(pinNr, PUPD_DISABLE);

		ADC_RegularChannelConfig(adcDesc.ADCx, channel, 1, ADC_SampleTime_480Cycles);

		ADC_SoftwareStartConv(adcDesc.ADCx);

		if (pdTRUE == xSemaphoreTake(adc.semHandle, portMAX_DELAY))
		{
			xSemaphoreGive(adc.mutexHandle);
			return ADC_GetConversionValue(adcDesc.ADCx);
		}
		else
		{
			xSemaphoreGive(adc.mutexHandle);
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

extern "C" void ADC_IRQHandler(void)
{
	signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	for (int i = 0; i < ADC_CNT; i++)
	{
		const adc_desc_t& adcDesc = adc_desc[i];
	        adc_data_t& adcData = adc_data[i];

		if (ADC_GetITStatus(adcDesc.ADCx, ADC_IT_EOC) != RESET)
		{
			xSemaphoreGiveFromISR(adcData.semHandle, &xHigherPriorityTaskWoken);

			ADC_ClearITPendingBit(adcDesc.ADCx, ADC_IT_EOC);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			return;
		}
	}
}

