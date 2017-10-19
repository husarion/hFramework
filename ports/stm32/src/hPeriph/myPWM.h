/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef MYPWM_H
#define MYPWM_H

#include "hTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

int servo_active[6] = {0, 0, 0, 0, 0, 0};

enum
{
#if BOARD(ROBOCORE)
	MYPWM_PIN_HMOT1 = 6,
	MYPWM_PIN_HMOT2 = 7,
	MYPWM_PIN_HMOT3 = 8,
	MYPWM_PIN_HMOT4 = 9,
	MYPWM_PIN_HMOT5 = 10,
	MYPWM_PIN_HMOT6 = 11,
#elif BOARD(CORE2)
	MYPWM_PIN_HMOT1 = 6,
	MYPWM_PIN_HMOT2 = 7,
	MYPWM_PIN_HMOT3 = 8,
	MYPWM_PIN_HMOT4 = 9,
	MYPWM_PIN_SERVO1 = 0,
	MYPWM_PIN_SERVO2 = 1,
	MYPWM_PIN_SERVO3 = 2,
	MYPWM_PIN_SERVO4 = 3,
	MYPWM_PIN_SERVO5 = 4,
	MYPWM_PIN_SERVO6 = 5,
#elif BOARD(CORE2MINI)
	MYPWM_PIN_HMOT1 = 6,
	MYPWM_PIN_HMOT2 = 7,
	MYPWM_PIN_SERVO1 = 0,
	MYPWM_PIN_SERVO2 = 1,
	MYPWM_PIN_SERVO3 = 2,
	MYPWM_PIN_SERVO4 = 3,
#else
#error no pwm
#endif
};

typedef enum
{
	PWM_POLARITY_LOW,
	PWM_POLARITY_HIGH
} pwm_polarity_t;

typedef enum
{
	PWM_FREQ_21_KHZ = 0,
	PWM_FREQ_12_KHZ,
	PWM_FREQ_6_KHZ,
	PWM_FREQ_4_KHZ,
	PWM_FREQ_3_KHZ,
	PWM_FREQ_2_KHZ,
	PWM_FREQ_1_KHZ,
	PWM_FREQ_500_HZ,
	PWM_FREQ_300_HZ,
	PWM_FREQ_200_HZ,
	PWM_FREQ_100_HZ,
	PWM_FREQ_50_HZ,
	PWM_FREQ_25_HZ,
	PWM_FREQ_10_HZ,
	PWM_FREQ_5_HZ,
	PWM_FREQ_1_HZ
} pwm_freq_val_t;

void myPWM_init(uint8_t myPWM_pin, pwm_polarity_t polarity);
void myPWM_acquireGPIO(uint8_t myPWM_pin);
void myPWM_releaseGPIO(uint8_t myPWM_pin);
void myPWM_setCnt_us(uint8_t myPWM_pin, uint16_t point_us);
void myPWM_setWidth_us(uint8_t myPWM_pin, uint16_t width_us);
void myPWM_setPeriod_us(uint8_t myPWM_pin, uint16_t period_us);
void myPWM_setCnt_ns(uint8_t myPWM_pin, uint16_t point_ns);
void myPWM_setWidth_ns(uint8_t myPWM_pin, uint16_t width_ns);
uint16_t myPWM_getWidth_ns(uint8_t myPWM_pin);
void myPWM_setPeriod_ns(uint8_t myPWM_pin, uint16_t period_ns);
void myPWM_counter_disable(uint8_t myPWM_pin);
void myPWM_counter_enable(uint8_t myPWM_pin);
void myPWM_counter_enable_all();

void myPWM_fix_freq_init(uint8_t myPWM_pin, pwm_freq_val_t freq);
void myPWM_fix_freq_duty(uint8_t myPWM_pin, uint16_t duty);	//set 0 - 1000

#ifdef __cplusplus
}
#endif

#endif /* MYPWM_H */
