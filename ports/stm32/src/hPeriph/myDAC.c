/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "myUSART.h"
#include "myADC.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R2_ADDRESS    0x4000741C

extern const uint8_t nagranie[34304];

static void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	/* TIM6 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* --------------------------------------------------------
	 	   TIM3 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1),
	 	   since APB1 prescaler is different from 1.
	   	 	 TIM6CLK = 2 * PCLK1
	   	 	 TIM6CLK = HCLK / 2 = SystemCoreClock /2

	 	   TIM6 Update event occurs each TIM6CLK/256

	 	   Note:
	  	SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	  	Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	  	function to update SystemCoreClock variable value. Otherwise, any configuration
	  	based on this variable will be incorrect.
		----------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0x1FFF >> 2; //todo
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	/* TIM6 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);

}

const uint16_t aSine12bit[32] =
{
	2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
	3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
	599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
};



//void myDAC_config(uint8_t pinNr)
//{
//	static DAC_InitTypeDef  DAC_InitStructure;	//to musialo byc poza stosem funkcji myDAC_confi :todo
//	DMA_InitTypeDef DMA_InitStructure;
//
//	/* Preconfiguration before using DAC----------------------------------------*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/* DMA1 clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
//	/* GPIOA clock enable (to be used with DAC) */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	/* DAC Periph clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//
//	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* TIM6 Configuration ------------------------------------------------------*/
//	TIM6_Config();
//
//	DAC_DeInit();
//
//
//
//	/* DAC channel2 Configuration */
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
//
//	/* DMA1_Stream6 channel7 configuration **************************************/
//	DMA_DeInit(DMA1_Stream6);
//	DMA_InitStructure.DMA_Channel = DMA_Channel_7;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R2_ADDRESS;//(uint32_t)DAC_DHR12R2_ADDRESS;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&nagranie;//(uint32_t)&aSine12bit;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	DMA_InitStructure.DMA_BufferSize = sizeof(nagranie);//32;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
//
//	/* Enable DMA1_Stream6 */
//	DMA_Cmd(DMA1_Stream6, ENABLE);
//
//	/* Enable DAC Channel2 */
//	DAC_Cmd(DAC_Channel_2, ENABLE);
//
//	/* Enable DMA for DAC Channel2 */
//	DAC_DMACmd(DAC_Channel_2, ENABLE);
//}

volatile DAC_InitTypeDef  DAC_InitStructure;	//to musialo byc poza stosem funkcji myDAC_confi :todo
volatile DMA_InitTypeDef DMA_InitStructure;
void myDAC_Init(void)
{
	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* DMA1 clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	/* GPIOA clock enable (to be used with DAC) */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* TIM6 Configuration ------------------------------------------------------*/
	TIM6_Config();

	DAC_DeInit();

	/* DAC channel2 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	/* DMA1_Stream6 channel7 configuration **************************************/
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R2_ADDRESS;//(uint32_t)DAC_DHR12R2_ADDRESS;
	//DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;//(uint32_t)&aSine12bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//DMA_InitStructure.DMA_BufferSize = size;//32;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	//	/* Enable DMA1_Stream6 */
	//	DMA_Cmd(DMA1_Stream6, ENABLE);
	//
	//	/* Enable DAC Channel2 */
	//	DAC_Cmd(DAC_Channel_2, ENABLE);
	//
	//	/* Enable DMA for DAC Channel2 */
	//	DAC_DMACmd(DAC_Channel_2, ENABLE);
}

void myDAC_playWav(uint8_t* data, uint32_t size)
{
	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
	{
		//printf_("d");
	}

	/* DMA1_Stream6 channel7 configuration **************************************/
	//	DMA_DeInit(DMA1_Stream6);

#define TRANSFER_IT_ENABLE_MASK (uint32_t)(DMA_SxCR_TCIE | DMA_SxCR_HTIE | \
		DMA_SxCR_TEIE | DMA_SxCR_DMEIE)

#define DMA_Stream0_IT_MASK     (uint32_t)(DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 | \
		DMA_LISR_TEIF0 | DMA_LISR_HTIF0 | \
		DMA_LISR_TCIF0)

#define DMA_Stream1_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 6)
#define DMA_Stream2_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 16)
#define DMA_Stream3_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 22)
#define DMA_Stream4_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream5_IT_MASK     (uint32_t)(DMA_Stream1_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream6_IT_MASK     (uint32_t)(DMA_Stream2_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream7_IT_MASK     (uint32_t)(DMA_Stream3_IT_MASK | (uint32_t)0x20000000)

	DMA1_Stream6->CR &= ~((uint32_t)DMA_SxCR_EN);
	DMA1->HIFCR = (uint32_t)DMA_Stream6_IT_MASK;
	DMA1_Stream6->M0AR = (uint32_t)data;
	DMA1_Stream6->NDTR = size;

	//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;//(uint32_t)&aSine12bit;
	//	DMA_InitStructure.DMA_BufferSize = size;//32;
	//	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	/* Enable DMA1_Stream6 */
	//DMA_Cmd(DMA1_Stream6, ENABLE);
	DMA1_Stream6->CR |= (uint32_t)DMA_SxCR_EN;

	/* Enable DAC Channel2 */
	//DAC_Cmd(DAC_Channel_2, ENABLE);
	DAC->CR |= (DAC_CR_EN1 << DAC_Channel_2);

	/* Enable DMA for DAC Channel2 */
	//DAC_DMACmd(DAC_Channel_2, ENABLE);
	DAC->CR |= (DAC_CR_DMAEN1 << DAC_Channel_2);

//	vTaskDelay(10*2);
//	while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
//	{
//		//printf_("d");
//	}
}



