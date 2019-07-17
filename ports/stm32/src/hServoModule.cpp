/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hServoModule.h"

#ifdef SERVOMODULE_ENABLED

#include <myGPIO.h>
#include <myPWM.h>
#include <hGPIO.h>

#include <peripherals.h>

#include "debug.h"
#include "ycm.h"

namespace hFramework
{

static hRecursiveMutex mutex;

#if BOARD(CORE2)
static hGPIO pinSel1(MYGPIO_SERVO_SEL1);
static hGPIO pinSel2(MYGPIO_SERVO_SEL2);
#endif
static hGPIO pinEnable(MYGPIO_SERVO_POWER_ON);
static hGPIO_adc pinSense(MYGPIO_SERVO_ADC);

static uint8_t pins[] =
{
#if BOARD(CORE2)
	MYPWM_PIN_SERVO1,
	MYPWM_PIN_SERVO2,
	MYPWM_PIN_SERVO3,
	MYPWM_PIN_SERVO4,
	MYPWM_PIN_SERVO5,
	MYPWM_PIN_SERVO6,
#elif BOARD(CORE2MINI)
	MYPWM_PIN_SERVO1,
	MYPWM_PIN_SERVO2,
	MYPWM_PIN_SERVO3,
	MYPWM_PIN_SERVO4,
#else
#  error no board
#endif
};

// ***************************************************************
// ********************* hServoModule_Servo **********************
// ***************************************************************
hServoModule_Servo::hServoModule_Servo(int num) : num(num)
{
}

void hServoModule_Servo::setWidth(uint16_t widthUs)
{
	hServoModule.setWidth(num - 1, widthUs);
}
void hServoModule_Servo::setPeriod(uint16_t periodUs)
{
	hServoModule.setPeriod(num - 1, periodUs);
}

// ***************************************************************
// ************************ hServoModule *************************
// ***************************************************************
hServoModuleClass::hServoModuleClass()
#if BOARD(CORE2)
	: servo1(1), servo2(2), servo3(3), servo4(4), servo5(5), servo6(6)
#elif BOARD(CORE2MINI)
	: servo1(1), servo2(2), servo3(3), servo4(4)
#else
#  error no board
#endif
{
}

void hServoModuleClass::init()
{
	SYSLOG("initializing hServoModule");

#if BOARD(CORE2)
	pinSel1.init();
	pinSel2.init();
	pinSel1.setOut();
	pinSel2.setOut();
#endif
	pinEnable.init();
	pinSense.init();
	pinEnable.setOut();
	pinSense.enableADC();

	for (int i = 0; i < SERVOS_COUNT; i++)
	{
		int pinPWM = pins[i];

		myPWM_init(pinPWM, PWM_POLARITY_LOW);

		myPWM_setCnt_ns(pinPWM, 0);
		myPWM_setPeriod_us(pinPWM, 20000);
		myPWM_counter_enable(pinPWM);
	}
}

void hServoModuleClass::setWidth(int num, uint16_t widthUs)
{
	if (num >= SERVOS_COUNT)
		return;
	int pinPWM = pins[num];
	myPWM_setWidth_us(pinPWM, widthUs);
}

void hServoModuleClass::setPeriod(int num, uint16_t periodUs)
{
	if (num >= SERVOS_COUNT) {
		for (int i = 0; i < SERVOS_COUNT; i++)
		{
			int pinPWM = pins[i];
			myPWM_setPeriod_us(pinPWM, periodUs);
		}
		return;
	}
		
	int pinPWM = pins[num];
	myPWM_setPeriod_us(pinPWM, periodUs);
}

void hServoModuleClass::enablePower()
{
	pinEnable.write(1);
}
void hServoModuleClass::disablePower()
{
	pinEnable.write(0);
}

float hServoModuleClass::getVoltage()
{
	const int UPPER_RESISTOR = 56; // kOhm
	const int LOWER_RESISTOR = 15; // kOhm

	float voltage = pinSense.analogReadVoltage();

	return voltage * (UPPER_RESISTOR + LOWER_RESISTOR) / LOWER_RESISTOR;
}
uint32_t hServoModuleClass::getVoltageMV()
{
	return getVoltage() * 1000;
}

#if BOARD(CORE2)
void hServoModuleClass::setPowerMode(int mode)
{
	switch (mode)
	{
	default:
	case 0: pinSel1.write(0); pinSel2.write(0); break;
	case 1: pinSel1.write(1); pinSel2.write(0); break;
	case 2: pinSel1.write(0); pinSel2.write(1); break;
	case 3: pinSel1.write(1); pinSel2.write(1); break;
	}
}
#endif

hServoModule_Servo& hServoModuleClass::getServo(int num)
{
	switch (num)
	{
	default:
#if BOARD(CORE2)
	case 1: return servo1;
	case 2: return servo2;
	case 3: return servo3;
	case 4: return servo4;
	case 5: return servo5;
	case 6: return servo6;
#elif BOARD(CORE2MINI)
	case 1: return servo1;
	case 2: return servo2;
	case 3: return servo3;
	case 4: return servo4;
#else
#  error no board
#endif
	}
}

}

#endif
