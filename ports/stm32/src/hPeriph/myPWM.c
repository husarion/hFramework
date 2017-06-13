/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myPWM.h"
#include "myGPIO.h"
#include "stm32f4xx.h"
#include "hTypes.h"
#include "debug.h"

typedef void (*TIM_OCxInit_t)(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*RCC_APBxPeriphClockCmd_t)(uint32_t, FunctionalState);

typedef struct
{
	uint8_t pin;

	uint8_t GPIO_AF_TIMx;
	RCC_APBxPeriphClockCmd_t RCC_APBxPeriphClockCmd;
	uint32_t RCC_APBxPeriph_TIMy;
	TIM_TypeDef* TIMx;
	TIM_OCxInit_t TIM_OCxInit;
	uint8_t TIMq_CCRs;
} pwm_desc_t;

#define PWM_ENTRY(pin, apb, tim, ocnum) \
	{ pin, GPIO_AF_ ## tim, RCC_ ## apb ## PeriphClockCmd, RCC_ ## apb ## Periph_ ## tim, tim, TIM_OC ## ocnum ## Init, ocnum }

const pwm_desc_t pwms[] =
{
#if BOARD(ROBOCORE)
	PWM_ENTRY(MYGPIO_MOT1_PWM,   APB2,  TIM9, 2),
	PWM_ENTRY(MYGPIO_MOT2_PWM,   APB2,  TIM9, 1),
	PWM_ENTRY(MYGPIO_MOT3_PWM,   APB1, TIM14, 1),
	PWM_ENTRY(MYGPIO_MOT4_PWM,   APB1, TIM13, 1),
	PWM_ENTRY(MYGPIO_MOT5_PWM,   APB2, TIM11, 1),
	PWM_ENTRY(MYGPIO_MOT6_PWM,   APB2, TIM10, 1),
#elif BOARD(CORE2)
	PWM_ENTRY(MYGPIO_MOT1_PWM,   APB2, TIM10, 1),
	PWM_ENTRY(MYGPIO_MOT2_PWM,   APB2, TIM11, 1),
	PWM_ENTRY(MYGPIO_MOT3_PWM,   APB1, TIM13, 1),
	PWM_ENTRY(MYGPIO_MOT4_PWM,   APB1, TIM14, 1),
	PWM_ENTRY(MYGPIO_SERVO1_PWM, APB2,  TIM1, 1),
	PWM_ENTRY(MYGPIO_SERVO2_PWM, APB2,  TIM1, 2),
	PWM_ENTRY(MYGPIO_SERVO3_PWM, APB2,  TIM1, 3),
	PWM_ENTRY(MYGPIO_SERVO4_PWM, APB2,  TIM1, 4),
	PWM_ENTRY(MYGPIO_SERVO5_PWM, APB1, TIM12, 2),
	PWM_ENTRY(MYGPIO_SERVO6_PWM, APB1, TIM12, 1),
#elif BOARD(CORE2MINI)
	PWM_ENTRY(MYGPIO_MOT1_PWM,   APB2, TIM10, 1),
	PWM_ENTRY(MYGPIO_MOT2_PWM,   APB2, TIM11, 1),
	PWM_ENTRY(MYGPIO_SERVO1_PWM, APB2,  TIM8, 2),
	PWM_ENTRY(MYGPIO_SERVO2_PWM, APB2,  TIM8, 3),
	PWM_ENTRY(MYGPIO_SERVO3_PWM, APB1,  TIM4, 1),
	PWM_ENTRY(MYGPIO_SERVO4_PWM, APB1,  TIM4, 2),
#else
#  error no board
#endif
};
const int pwms_count = sizeof(pwms) / sizeof(pwms[0]);

typedef struct
{
	uint16_t PSC;
	uint16_t ARR;
} PWM_freq_param_t;

const PWM_freq_param_t PWM_freqs[] =
{
	/* 21 kHz */ { 7, 1000 },
	/* 12 kHz */ { 13, 1000 },
	/* 6  kHz */ { 27, 1000 },
	/* 4  kHz */ { 41, 1000 },
	/* 3  kHz */ { 55, 1000 },
	/* 2  kHz */ { 83, 1000 },
	/* 1  kHz */ { 167, 1000 },
	/* 500 Hz */ { 335, 1000 },
	/* 300 Hz */ { 559, 1000 },
	/* 200 Hz */ { 839, 1000 },
	/* 100 Hz */ { 1679, 1000 },
	/* 50  Hz */ { 3359, 1000 },
	/* 25  Hz */ { 6719, 1000 },
	/* 10  Hz */ { 1679, 10000 },
	/* 5   Hz */ { 1679, 20000 },
	/* 1   Hz */ { 3359, 50000 },
};

void myPWM_init(uint8_t myPWM_pin, pwm_polarity_t polarity)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;

	ASSERT(myPWM_pin < pwms_count);

	myPWM_acquireGPIO(myPWM_pin);

	/* TIM clock enable */
	pwms[myPWM_pin].RCC_APBxPeriphClockCmd(pwms[myPWM_pin].RCC_APBxPeriph_TIMy , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 20 ms 50 Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(pwms[myPWM_pin].TIMx, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	if (polarity == PWM_POLARITY_HIGH)
	{
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	}
	if (polarity == PWM_POLARITY_LOW)
	{
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	}
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	pwms[myPWM_pin].TIM_OCxInit(pwms[myPWM_pin].TIMx, &TIM_OCInitStructure);

	/* TIM counter enable */
	TIM_Cmd(pwms[myPWM_pin].TIMx, ENABLE);

	/* TIM Main Output Enable */
	TIM_CtrlPWMOutputs(pwms[myPWM_pin].TIMx, ENABLE);
}
void myPWM_acquireGPIO(uint8_t myPWM_pin)
{
	ASSERT(myPWM_pin < pwms_count);
	myGPIO_config_af(pwms[myPWM_pin].pin, pwms[myPWM_pin].GPIO_AF_TIMx);
}
void myPWM_releaseGPIO(uint8_t myPWM_pin)
{
	ASSERT(myPWM_pin < pwms_count);
	myGPIO_config(pwms[myPWM_pin].pin);
}
void myPWM_setCnt_us(uint8_t myPWM_pin, uint16_t point_us)
{
	ASSERT(myPWM_pin < pwms_count);
	pwms[myPWM_pin].TIMx->CNT = point_us;
}
void myPWM_setWidth_us(uint8_t myPWM_pin, uint16_t width_us)
{
	ASSERT(myPWM_pin < pwms_count);

	switch (pwms[myPWM_pin].TIMq_CCRs)
	{
	case 1: pwms[myPWM_pin].TIMx->CCR1 = width_us; break;
	case 2: pwms[myPWM_pin].TIMx->CCR2 = width_us; break;
	case 3: pwms[myPWM_pin].TIMx->CCR3 = width_us; break;
	case 4: pwms[myPWM_pin].TIMx->CCR4 = width_us; break;
	}
}

void myPWM_setPeriod_us(uint8_t myPWM_pin, uint16_t period_us)
{
	ASSERT(myPWM_pin < pwms_count);

	pwms[myPWM_pin].TIMx->PSC = 168 - 1;
	pwms[myPWM_pin].TIMx->EGR = TIM_PSCReloadMode_Immediate;
	pwms[myPWM_pin].TIMx->ARR = period_us;
}

//allways at first set period then pulse width
void myPWM_setCnt_ns(uint8_t myPWM_pin, uint16_t point_ns)
{
	ASSERT(myPWM_pin < pwms_count);
	pwms[myPWM_pin].TIMx->CNT = point_ns / 50;
}
void myPWM_setWidth_ns(uint8_t myPWM_pin, uint16_t width_ns)
{
	ASSERT(myPWM_pin < pwms_count);
	switch (pwms[myPWM_pin].TIMq_CCRs)
	{
	case 1: pwms[myPWM_pin].TIMx->CCR1 = width_ns / 50; break;
	case 2: pwms[myPWM_pin].TIMx->CCR2 = width_ns / 50; break;
	case 3: pwms[myPWM_pin].TIMx->CCR3 = width_ns / 50; break;
	case 4: pwms[myPWM_pin].TIMx->CCR4 = width_ns / 50; break;
	}
}
uint16_t myPWM_getWidth_ns(uint8_t myPWM_pin)
{
	ASSERT(myPWM_pin < pwms_count);
	switch (pwms[myPWM_pin].TIMq_CCRs)
	{
	default:
	case 1: return pwms[myPWM_pin].TIMx->CCR1 * 50;
	case 2: return pwms[myPWM_pin].TIMx->CCR2 * 50;
	case 3: return pwms[myPWM_pin].TIMx->CCR3 * 50;
	case 4: return pwms[myPWM_pin].TIMx->CCR4 * 50;
	}
}

void myPWM_setPeriod_ns(uint8_t myPWM_pin, uint16_t period_ns)
{
	ASSERT(myPWM_pin < pwms_count);

	pwms[myPWM_pin].TIMx->PSC = 8 - 1;
	pwms[myPWM_pin].TIMx->EGR = TIM_PSCReloadMode_Immediate;
	pwms[myPWM_pin].TIMx->ARR = period_ns / 50;
}

extern void myPWM_counter_disable(uint8_t myPWM_pin)
{
	ASSERT(myPWM_pin < pwms_count);
	pwms[myPWM_pin].TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN;
}
extern void myPWM_counter_enable(uint8_t myPWM_pin)
{
	ASSERT(myPWM_pin < pwms_count);
	pwms[myPWM_pin].TIMx->CR1 |= (uint16_t)TIM_CR1_CEN;
}

void myPWM_counter_enable_all()
{
	volatile uint16_t* M1_CR1 = &pwms[MYPWM_PIN_HMOT1].TIMx->CR1;
	volatile uint16_t* M2_CR1 = &pwms[MYPWM_PIN_HMOT2].TIMx->CR1;

	*M1_CR1 |= TIM_CR1_CEN;
	*M2_CR1 |= TIM_CR1_CEN;
}

void myPWM_fix_freq_init(uint8_t myPWM_pin, pwm_freq_val_t freq)
{
	ASSERT(myPWM_pin < pwms_count);

	pwms[myPWM_pin].TIMx->PSC = PWM_freqs[freq].PSC;
	pwms[myPWM_pin].TIMx->EGR = TIM_PSCReloadMode_Immediate;
	pwms[myPWM_pin].TIMx->ARR = PWM_freqs[freq].ARR;
}
void myPWM_fix_freq_duty(uint8_t myPWM_pin, uint16_t duty)
{
	ASSERT(myPWM_pin < pwms_count);

	if (duty > 1000)
		duty = 1000;

	switch (pwms[myPWM_pin].TIMq_CCRs)
	{
	case 1: pwms[myPWM_pin].TIMx->CCR1 = (pwms[myPWM_pin].TIMx->ARR / 1000) * duty; break;
	case 2: pwms[myPWM_pin].TIMx->CCR2 = (pwms[myPWM_pin].TIMx->ARR / 1000) * duty; break;
	case 3: pwms[myPWM_pin].TIMx->CCR3 = (pwms[myPWM_pin].TIMx->ARR / 1000) * duty; break;
	case 4: pwms[myPWM_pin].TIMx->CCR4 = (pwms[myPWM_pin].TIMx->ARR / 1000) * duty; break;
	}
}
