/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <myEncoder.h>
#include <myGPIO.h>
#include <hTypes.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"

#include <port.h>

#define ENCODER_ENTRY(gpioA, pinA, gpioB, pinB, apb, tim, irq) { \
	gpioA, RCC_AHB1Periph_ ## gpioA, GPIO_Pin_ ## pinA, GPIO_PinSource ## pinA, \
	gpioB, RCC_AHB1Periph_ ## gpioB, GPIO_Pin_ ## pinB, GPIO_PinSource ## pinB, \
	GPIO_AF_ ## tim, RCC_ ## apb ## PeriphClockCmd, RCC_ ## apb ## Periph_ ## tim, \
	tim, irq }

// Constatnt info for each encoder
typedef struct
{
	GPIO_TypeDef* GPIOy_MxA_out;
	uint32_t RCC_AHB1Periph_GPIOy_MxA_out;
	uint16_t GPIO_Pin_x_MxA_out;
	uint8_t GPIO_PinSourcex_MxA_out;

	GPIO_TypeDef* GPIOy_MxB_out;
	uint32_t RCC_AHB1Periph_GPIOy_MxB_out;
	uint16_t GPIO_Pin_x_MxB_out;
	uint8_t GPIO_PinSourcex_MxB_out;

	uint8_t GPIO_AF_TIMq;

	void (*RCC_APBrPeriphClockCmd)(uint32_t, FunctionalState);
	uint32_t RCC_APBrPeriph_TIMq;
	TIM_TypeDef* TIMq;
	uint8_t NVIC_IRQChannel_timq;
} encoder_desc_t;

const encoder_desc_t encoder_desc[] =
{
#if BOARD(ROBOCORE)
	/* MYENCODER_HMOT1 */ ENCODER_ENTRY(GPIOE,  9, GPIOE, 11, APB2, TIM1, TIM1_UP_TIM10_IRQn),
	/* MYENCODER_HMOT2 */ ENCODER_ENTRY(GPIOA, 15, GPIOB,  3, APB1, TIM2, TIM2_IRQn),
	/* MYENCODER_HMOT3 */ ENCODER_ENTRY(GPIOA,  0, GPIOA,  1, APB1, TIM5, TIM5_IRQn),
	/* MYENCODER_HMOT4 */ ENCODER_ENTRY(GPIOC,  6, GPIOC,  7, APB2, TIM8, TIM8_UP_TIM13_IRQn),
	/* MYENCODER_HMOT5 */ ENCODER_ENTRY(GPIOB,  4, GPIOA,  7, APB1, TIM3, TIM3_IRQn),
	/* MYENCODER_HMOT6 */ ENCODER_ENTRY(GPIOD, 12, GPIOD, 13, APB1, TIM4, TIM4_IRQn),
#elif BOARD(CORE2)
	/* MYENCODER_HMOT1 */ ENCODER_ENTRY(GPIOA,  0, GPIOA,  1, APB1, TIM5, TIM5_IRQn),
	/* MYENCODER_HMOT2 */ ENCODER_ENTRY(GPIOC,  6, GPIOC,  7, APB2, TIM8, TIM8_UP_TIM13_IRQn),
	/* MYENCODER_HMOT3 */ ENCODER_ENTRY(GPIOB,  4, GPIOA,  7, APB1, TIM3, TIM3_IRQn),
	/* MYENCODER_HMOT4 */ ENCODER_ENTRY(GPIOB,  6, GPIOB,  7, APB1, TIM4, TIM4_IRQn),
#elif BOARD(CORE2MINI)
	/* MYENCODER_HMOT1 */ ENCODER_ENTRY(GPIOA,  1, GPIOA,  0, APB1, TIM5, TIM5_IRQn),
	/* MYENCODER_HMOT2 */ ENCODER_ENTRY(GPIOA,  7, GPIOA,  6, APB1, TIM3, TIM3_IRQn),
#else
#  error no board
#endif
};

///////////////////// hMotor OVF interrupt /////////////////////////////////

int16_t higherBits[COUNT_OF(encoder_desc)] = { 0 };
const int32_t MAX_TIMER_VALUE_HALF = 65536 / 2;

void timX_irqHandler(TIM_TypeDef* tim, uint8_t motorNr)
{
	if (TIM_GetITStatus(tim, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(tim, TIM_IT_Update);
		if (tim->CNT < MAX_TIMER_VALUE_HALF)
			higherBits[motorNr]++;
		else
			higherBits[motorNr]--;
	}
}

void myEncoder_setPullup(uint8_t motorNr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxA_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxA_out, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxB_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxB_out, &GPIO_InitStructure);
}

void myEncoder_setPulldown(uint8_t motorNr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxA_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxA_out, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxB_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxB_out, &GPIO_InitStructure);
}

void myEncoder_setPullReset(uint8_t motorNr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxA_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxA_out, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxB_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxB_out, &GPIO_InitStructure);
}

void myEncoder_init(uint8_t motorNr)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	encoder_desc[motorNr].RCC_APBrPeriphClockCmd(encoder_desc[motorNr].RCC_APBrPeriph_TIMq, ENABLE);
	// GPIO_PinAFConfig(encoder_desc[motorNr].GPIOy_MxA_out, encoder_desc[motorNr].GPIO_PinSourcex_MxA_out, encoder_desc[motorNr].GPIO_AF_TIMq);
	// GPIO_PinAFConfig(encoder_desc[motorNr].GPIOy_MxB_out, encoder_desc[motorNr].GPIO_PinSourcex_MxB_out, encoder_desc[motorNr].GPIO_AF_TIMq);

	RCC_AHB1PeriphClockCmd(encoder_desc[motorNr].RCC_AHB1Periph_GPIOy_MxA_out, ENABLE);
	RCC_AHB1PeriphClockCmd(encoder_desc[motorNr].RCC_AHB1Periph_GPIOy_MxB_out, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxA_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxA_out, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = encoder_desc[motorNr].GPIO_Pin_x_MxB_out;
	GPIO_Init(encoder_desc[motorNr].GPIOy_MxB_out, &GPIO_InitStructure);

	GPIO_PinAFConfig(encoder_desc[motorNr].GPIOy_MxA_out, encoder_desc[motorNr].GPIO_PinSourcex_MxA_out, encoder_desc[motorNr].GPIO_AF_TIMq);
	GPIO_PinAFConfig(encoder_desc[motorNr].GPIOy_MxB_out, encoder_desc[motorNr].GPIO_PinSourcex_MxB_out, encoder_desc[motorNr].GPIO_AF_TIMq);

	TIM_EncoderInterfaceConfig(encoder_desc[motorNr].TIMq, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ITConfig(encoder_desc[motorNr].TIMq, TIM_IT_Update, ENABLE);

	TIM_Cmd(encoder_desc[motorNr].TIMq, ENABLE);

	encoder_desc[motorNr].TIMq->ARR = 0xFFFF;

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = encoder_desc[motorNr].NVIC_IRQChannel_timq;

	portENTER_CRITICAL();
	NVIC_Init(&NVIC_InitStructure);

	encoder_desc[motorNr].TIMq->CNT = 0;
	TIM_ClearITPendingBit(encoder_desc[motorNr].TIMq, TIM_IT_Update);
	portEXIT_CRITICAL();

}
void myEncoder_setPolarity(uint8_t motorNr, uint8_t polarity)
{
	TIM_EncoderInterfaceConfig(
	  encoder_desc[motorNr].TIMq,
	  TIM_EncoderMode_TI12,
	  polarity == 0 ? TIM_ICPolarity_Rising : TIM_ICPolarity_Falling,
	  TIM_ICPolarity_Rising);
}

int32_t myEncoder_getCnt(uint8_t motorNr)
{
	TIM_TypeDef *tim = encoder_desc[motorNr].TIMq;
	uint16_t timerCnt;
	int32_t highBits;

	/* possible fail scenario:
	 * 1. timer counter = 65535 (one to overflow)
	 * 2. program execution enter to critical section
	 * 3. BEFORE timer value is read, timer overflowed, CNT is now 0, higher bits have not been updated due to critical section
	 * 4. new timer value is read (0), but higherBits array has old data still
	 * 5. summation of these values gives nonsense
	 *
	 * possible solution: check timer interrupt flag AFTER reading timer CNT, manage properly and reread CNT register
	 */

	portENTER_CRITICAL();
	timerCnt = tim->CNT;

	if (TIM_GetITStatus(tim, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(tim, TIM_IT_Update);
		if (tim->CNT < MAX_TIMER_VALUE_HALF)
			higherBits[motorNr]++;
		else
			higherBits[motorNr]--;
		timerCnt = tim->CNT;
	}

	highBits = higherBits[motorNr];
	portEXIT_CRITICAL();
	return (highBits << 16) | timerCnt;
}

void myEncoder_resetCnt(uint8_t motorNr)
{
	TIM_TypeDef *tim = encoder_desc[motorNr].TIMq;

	portENTER_CRITICAL();
	tim->CNT = 0;
	higherBits[motorNr] = 0;
	// clearing timer overflow flag as it may have overflowed between entering critical section and reseting timer CNT.
	TIM_ClearITPendingBit(tim, TIM_IT_Update); 
	portEXIT_CRITICAL();
}

#define ENCODER_IRQ(_irq, _tim, _id) \
	void _irq ## _IRQHandler() { timX_irqHandler(_tim, _id); }

#if BOARD(ROBOCORE)
ENCODER_IRQ(TIM1_UP_TIM10, TIM1, MYENCODER_HMOT1)
ENCODER_IRQ(TIM2,          TIM2, MYENCODER_HMOT2)
ENCODER_IRQ(TIM5,          TIM5, MYENCODER_HMOT3)
ENCODER_IRQ(TIM8_UP_TIM13, TIM8, MYENCODER_HMOT4)
ENCODER_IRQ(TIM3,          TIM3, MYENCODER_HMOT5)
ENCODER_IRQ(TIM4,          TIM4, MYENCODER_HMOT6)
#elif BOARD(CORE2)
ENCODER_IRQ(TIM5,          TIM5, MYENCODER_HMOT1)
ENCODER_IRQ(TIM8_UP_TIM13, TIM8, MYENCODER_HMOT2)
ENCODER_IRQ(TIM3,          TIM3, MYENCODER_HMOT3)
ENCODER_IRQ(TIM4,          TIM4, MYENCODER_HMOT4)
#elif BOARD(CORE2MINI)
ENCODER_IRQ(TIM5,          TIM5, MYENCODER_HMOT1)
ENCODER_IRQ(TIM3,          TIM3, MYENCODER_HMOT2)
#else
#  error no board
#endif
