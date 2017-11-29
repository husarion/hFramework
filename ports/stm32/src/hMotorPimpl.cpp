/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hMotorPimpl.h"

#include "myGPIO.h"
#include "myPWM.h"
#include "myEncoder.h"
#include "hUtils.h"

#define HMOTORPIMPL_DEBUG
#define DEBUG_CATEGORY HMOTORPIMPL
#include "debug.h"

#include "FreeRTOS.h"

#include "hPIDRegulator.h"

#include <port.h>

namespace hFramework
{

uint8_t driverModes[] = { DRIVER_MODE_NONE, DRIVER_MODE_NONE, DRIVER_MODE_NONE };
hPIDRegulator defaultPosRegulator[ROBOCORE_HMOTOR_CNT];
hMotorPimpl pimpls[ROBOCORE_HMOTOR_CNT];

const TMotorDef hMotorDefs[] =
{
#if BOARD(ROBOCORE)
	{ MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, MYPWM_PIN_HMOT1, MYENCODER_HMOT1, 0 },
	{ MYGPIO_MOT2A_IN, MYGPIO_MOT2B_IN, MYPWM_PIN_HMOT2, MYENCODER_HMOT2, 0 },
	{ MYGPIO_MOT3A_IN, MYGPIO_MOT3B_IN, MYPWM_PIN_HMOT3, MYENCODER_HMOT3, 1 },
	{ MYGPIO_MOT4A_IN, MYGPIO_MOT4B_IN, MYPWM_PIN_HMOT4, MYENCODER_HMOT4, 1 },
	{ MYGPIO_MOT5A_IN, MYGPIO_MOT5B_IN, MYPWM_PIN_HMOT5, MYENCODER_HMOT5, 2 },
	{ MYGPIO_MOT6A_IN, MYGPIO_MOT6B_IN, MYPWM_PIN_HMOT6, MYENCODER_HMOT6, 2 },
#elif BOARD(CORE2)
	{ MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, MYPWM_PIN_HMOT1, MYENCODER_HMOT1, 0 },
	{ MYGPIO_MOT2A_IN, MYGPIO_MOT2B_IN, MYPWM_PIN_HMOT2, MYENCODER_HMOT2, 0 },
	{ MYGPIO_MOT3A_IN, MYGPIO_MOT3B_IN, MYPWM_PIN_HMOT3, MYENCODER_HMOT3, 1 },
	{ MYGPIO_MOT4A_IN, MYGPIO_MOT4B_IN, MYPWM_PIN_HMOT4, MYENCODER_HMOT4, 1 },
#elif BOARD(CORE2MINI)
	{ MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, MYPWM_PIN_HMOT1, MYENCODER_HMOT1, 0 },
	{ MYGPIO_MOT2A_IN, MYGPIO_MOT2B_IN, MYPWM_PIN_HMOT2, MYENCODER_HMOT2, 0 },
#else
#  error no board
#endif
};

const TDriverDef hDriverDefs[] =
{
#if BOARD(ROBOCORE)
	{ MYGPIO_MOT12_SLEEP, MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, 0, 1 },
	{ MYGPIO_MOT34_SLEEP, MYGPIO_MOT3A_IN, MYGPIO_MOT3B_IN, 2, 3 },
	{ MYGPIO_MOT56_SLEEP, MYGPIO_MOT5A_IN, MYGPIO_MOT5B_IN, 4, 5 },
#elif BOARD(CORE2)
	{ MYGPIO_MOT12_SLEEP, MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, 0, 1 },
	{ MYGPIO_MOT34_SLEEP, MYGPIO_MOT3A_IN, MYGPIO_MOT3B_IN, 2, 3 },
#elif BOARD(CORE2MINI)
	{ MYGPIO_MOT12_SLEEP, MYGPIO_MOT1A_IN, MYGPIO_MOT1B_IN, 0, 1 },
#else
#  error no board
#endif
};

void switchDriverMode(int driverId, int newMode)
{
	hGPIO pinSleep(hDriverDefs[driverId].pinSleep);
	hGPIO pinModeA(hDriverDefs[driverId].pinModeA);
	hGPIO pinModeB(hDriverDefs[driverId].pinModeB);
	pinSleep.init();
	pinModeA.init();
	pinModeB.init();

	LOG("putting driver %d into sleep mode", driverId);
	pinSleep.setOut();
	pinSleep.write(0);
	sys.delay(2);

	switch (newMode)
	{
	case DRIVER_MODE_SINGLE:
		LOG("sel single mode");
		pinModeA.setOut();
		pinModeB.setOut();
		break;
	case DRIVER_MODE_PARALLEL:
		LOG("sel parallel mode");
		pinModeA.setIn();
		pinModeB.setIn();
		break;
	}

	pinSleep.write(1);
	sys.delay(2);
	LOG("awaking driver %d", driverId);
	pinModeA.setIn();
	pinModeB.setIn();

	driverModes[driverId] = newMode;
}

void hMotorPimpl::setupDrivers()
{
#if BOARD(ROBOCORE)
	switchDriverMode(0, DRIVER_MODE_SINGLE);
	switchDriverMode(1, DRIVER_MODE_SINGLE);
	switchDriverMode(2, DRIVER_MODE_SINGLE);
#elif BOARD(CORE2)
	switchDriverMode(0, DRIVER_MODE_SINGLE);
	switchDriverMode(1, DRIVER_MODE_SINGLE);
#elif BOARD(CORE2MINI)
	switchDriverMode(0, DRIVER_MODE_SINGLE);
#else
#  error no board
#endif
}
void hMotorPimpl::setDefaults()
{
	LOG("setting defaults");

#if BOARD(ROBOCORE)
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_1_2, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_3, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_4, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_5, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_6, PWM_freq_21_kHz);
#elif BOARD(CORE2)
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_1, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_2, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_3, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_4, PWM_freq_21_kHz);
#elif BOARD(CORE2MINI)
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_1, PWM_freq_21_kHz);
	hMotor::setPWMFreq(PWM_freq_hMotor_ID_2, PWM_freq_21_kHz);
#else
#  error no board
#endif
}

bool hMotorPimpl::init()
{
	hPIDRegulator& reg = defaultPosRegulator[motorId];
	mode = hMotorPimpl::MOTOR;
	limit = 1000;

	regPosition = &reg;
	regTaskRunning = false;
	regTargetEncoder = 0;
	regulationDone = true;

	slewTaskRunning = false;
	slewRate = 0;

	curPower = 0;

	LOG("init motor %d", motorId);

	myGPIO_config(hMotorDefs[motorId].pinMotA);
	myGPIO_config(hMotorDefs[motorId].pinMotB);

	myPWM_init(hMotorDefs[motorId].pinPWM, PWM_POLARITY_HIGH);

	motorPolarity = Polarity::Normal;
	encoderPolarity = Polarity::Normal;

	myEncoder_init(hMotorDefs[motorId].encoderNr);

	// default regulator
	// hPID
	reg.setScale(1);
	reg.setKP(40.0);
	reg.setKI(0.05);
	reg.setKD(1000);
	// hRegulator
	reg.dtMs = 5;
	reg.stableRange = 10;
	reg.stableTimes = 3;

	activeBreaking = true;

	controlMotorId = motorId;
	initialized = true;

	return true;
}

void hMotorPimpl::setPower(int16_t power)
{
	hMutexGuard guard(setPowerMutex);

	if (mode != hMotorPimpl::MOTOR)
		return;

	if (slewRate > 0)
		slewTarget = power;
	else
		updatePower_internal(false, power);
}
void hMotorPimpl::updatePower_internal(bool force, int16_t power)
{
	hMutexGuard guard(setPowerMutex);

	if (mode != hMotorPimpl::MOTOR)
		return;

	uint8_t dir;

	power = saturate(power, (int16_t) - limit, limit);

	int16_t tmpPower = power;
	if (tmpPower < 0)
	{
		dir = 1;
		tmpPower = -tmpPower;
	}
	else
	{
		dir = 0;
	}
	if (motorPolarity == Polarity::Reversed)
		dir = !dir;

	if (getDriverMode() == DRIVER_MODE_PARALLEL)
	{
		const TDriverDef& driver = getDriver();

		if (!force && power == pimpls[driver.controlMotorId].curPower && power == pimpls[driver.slaveMotorId].curPower)
			return;
		pimpls[driver.slaveMotorId].curPower = power;
		pimpls[driver.controlMotorId].curPower = power;
	}
	else
	{
		if (!force && power == curPower)
			return;
		curPower = power;
	}

	int pinA = hMotorDefs[controlMotorId].pinMotA;
	int pinB = hMotorDefs[controlMotorId].pinMotB;
	int pinPWM = hMotorDefs[controlMotorId].pinPWM;

	portDISABLE_INTERRUPTS();
	myPWM_counter_disable(pinPWM);

	if (tmpPower == 0)
	{
		myGPIO_dir_out(pinA);
		myGPIO_dir_out(pinB);
		if (activeBreaking)
		{
			myGPIO_write(pinA, 1);
			myGPIO_write(pinB, 1);
		}
		else
		{
			myGPIO_write(pinA, 0);
			myGPIO_write(pinB, 0);
		}
	}
	else
	{
		if (dir == 0)
		{
			if (activeBreaking)
			{
				myGPIO_dir_out(pinA);
				myGPIO_write(pinA, 1);
				myGPIO_dir_in(pinB);
			}
			else
			{
				myGPIO_dir_out(pinB);
				myGPIO_write(pinB, 0);
				myGPIO_dir_in(pinA);
				tmpPower = 1000 - tmpPower;
			}
		}
		else
		{
			if (activeBreaking)
			{
				myGPIO_dir_out(pinB);
				myGPIO_write(pinB, 1);
				myGPIO_dir_in(pinA);
			}
			else
			{
				myGPIO_dir_out(pinA);
				myGPIO_write(pinA, 0);
				myGPIO_dir_in(pinB);
				tmpPower = 1000 - tmpPower;
			}
		}
	}

	myPWM_setCnt_ns(pinPWM, 0);
	myPWM_fix_freq_duty(pinPWM, tmpPower);
	myPWM_counter_enable(pinPWM);
	portENABLE_INTERRUPTS();
}
void hMotorPimpl::setSlewRate(float time)
{
	hMutexGuard guard(accessMutex);

	slewRate = time * 100;
	if (slewRate > 0)
	{
		if (!slewTaskRunning)
		{
			char taskName[] = "hMotor_slewX";
			taskName[sizeof(taskName) - 2] = motorId + 1 + '0';
			slewTaskRunning = true;
			slewTaskExit = false;
			slewTask = &sys.taskCreate(std::bind(&hMotorPimpl::slewRateTask, this), 3, 400, taskName);
		}
	}
	else
	{
		if (!slewTaskRunning)
			return;

		slewTaskExit = true;
		slewTask->join();
	}
}
void hMotorPimpl::slewRateTask()
{
	uint32_t t = sys.getRefTime();
	while (!slewTaskExit)
	{
		int dt = 10;
		sys.delaySync(t, dt);

		int32_t diff = slewTarget - curPower;
		int32_t diffMult = diff * (int32_t)slewRate / 100;
		if (diff != 0 && diffMult == 0)
			diffMult = diff > 0 ? 1 : -1;

		int32_t newPower = curPower + diffMult;
		updatePower_internal(false, newPower);
	}

	LOG("slew task ended");
	slewTaskRunning = false;
}

// regulation
void hMotorPimpl::attachPositionRegulator(hRegulator& regPosition)
{
	this->regPosition = &regPosition;
}
bool hMotorPimpl::rotAbs(int32_t angle, uint16_t power, uint32_t timeout)
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return false;

	regTargetEncoder = angle;
	regPower = power;
	return doRotate(timeout);
}
bool hMotorPimpl::rotRel(int32_t angle, uint16_t power, uint32_t timeout)
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return false;

	if (!regTaskRunning)
	{
		hMotor* motor = port_motors[motorId];
		regTargetEncoder = motor->getEncoderCnt();
	}

	regTargetEncoder += angle;
	regPower = power;
	return doRotate(timeout);
}
bool hMotorPimpl::calibrate(int16_t power, uint32_t maxDiff, uint32_t stableTime, uint32_t timeout)
{
	// sys.log("pow %d %d %d\r\n", mop.param.calib.power, 0, 0);
	hMotor* motor = port_motors[motorId];
	motor->setPower(power);
	int32_t encoderCnt = motor->getEncoderCnt();
	uint32_t startTime = sys.getRefTime();
	uint32_t validTime;
	bool isValid = false;
	while (sys.getRefTime() - startTime < timeout)
	{
		uint32_t curTime = sys.getRefTime();
		int32_t curEncoderCnt = motor->getEncoderCnt();

		sys.delay(10);
		uint32_t curEnc = abs(curEncoderCnt - encoderCnt);
		// sys.log("q %d %d %d\r\n", curEnc, isValid, validTime);
		encoderCnt = curEncoderCnt;
		bool isOk = curEnc < maxDiff;
		if (!isValid && isOk)
		{
			isValid = true;
			validTime = curTime;
		}
		if (!isOk)
		{
			isValid = false;
		}

		if (isValid && curTime - validTime > stableTime)
		{
			motor->setPower(0);
			return true;
		}
	}
	motor->setPower(0);
	return false;
}
bool hMotorPimpl::doRotate(uint32_t timeout)
{
	regulationDone = false;
	operationDoneMutex.take(0);
	if (!regTaskRunning)
	{
		char taskName[] = "hMotor_regX";
		taskName[sizeof(taskName) - 2] = motorId + 1 + '0';
		regTaskRunning = true;
		regTaskExit = false;
		regTask = &sys.taskCreate(std::bind(&hMotorPimpl::regulatorTask, this), 3, 400, taskName);
	}

	if (timeout > 0)
		return waitDone(timeout);
	else
		return true;
}
bool hMotorPimpl::waitDone(uint32_t timeout)
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return false;

	if (regulationDone)
		return true;

	if (operationDoneMutex.take(timeout))
	{
		regulationDone = true;
		return true;
	}
	else
	{
		return false;
	}
}
bool hMotorPimpl::stopRegulation(bool block, uint32_t timeout)
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return false;

	if (!regTaskRunning)
		return true;

	regTaskExit = true;

	if (block)
		return regTask->join(timeout);
	else
		return true;
}
void hMotorPimpl::regulatorTask()
{
	hMotor* motor = port_motors[motorId];

	uint16_t dt = regPosition->dtMs;
	uint32_t t = sys.getRefTime();
	while (!regTaskExit)
	{
		sys.delaySync(t, dt);
		int32_t encoderCnt = motor->getEncoderCnt();
		int16_t pulse;
		int retVal = regPosition->regFunct(encoderCnt, regTargetEncoder, regPower, pulse);
		motor->setPower(pulse);

		switch (retVal)
		{
		case REG_IN_PROGRESS:
			break;
		case REG_FINISH_BREAK:
			break;
		case REG_FINISH_CONTINUE:
			operationDoneMutex.give();
			break;
		}
	}

	LOG("reg task ended");
	motor->setPower(0);
	regTaskRunning = false;
}


// parallel, single mode
void hMotorPimpl::setParallelMode()
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return;

	int driverId = hMotorDefs[motorId].driverId;
	int slaveMotorId = hDriverDefs[driverId].slaveMotorId;
	int controlMotorId = hDriverDefs[driverId].controlMotorId;

	if (driverModes[driverId] == DRIVER_MODE_PARALLEL)
		return;

	myPWM_releaseGPIO(hMotorDefs[slaveMotorId].pinPWM);
	switchDriverMode(driverId, DRIVER_MODE_PARALLEL);
	myPWM_acquireGPIO(hMotorDefs[slaveMotorId].pinPWM);

	pimpls[slaveMotorId].controlMotorId = controlMotorId;
	pimpls[controlMotorId].controlMotorId = controlMotorId;

	pimpls[controlMotorId].updatePower_internal(true, pimpls[motorId].curPower);
}

void hMotorPimpl::setPullup(){
	myEncoder_setPullup(hMotorDefs[motorId].encoderNr);
}
void hMotorPimpl::setPulldown(){
	myEncoder_setPulldown(hMotorDefs[motorId].encoderNr);
}
void hMotorPimpl::setPullReset(){
	myEncoder_setPullReset(hMotorDefs[motorId].encoderNr);
}

void hMotorPimpl::setSingleMode()
{
	hMutexGuard guard(accessMutex);

	if (mode != hMotorPimpl::MOTOR)
		return;

	int driverId = hMotorDefs[motorId].driverId;
	int slaveMotorId = hDriverDefs[driverId].slaveMotorId;
	int controlMotorId = hDriverDefs[driverId].controlMotorId;

	if (driverModes[driverId] == DRIVER_MODE_SINGLE)
		return;

	myPWM_releaseGPIO(hMotorDefs[controlMotorId].pinPWM);
	switchDriverMode(driverId, DRIVER_MODE_SINGLE);
	myPWM_acquireGPIO(hMotorDefs[controlMotorId].pinPWM);

	pimpls[slaveMotorId].controlMotorId = pimpls[slaveMotorId].motorId;
	pimpls[controlMotorId].controlMotorId = pimpls[controlMotorId].motorId;
}

void hMotorPimpl::setPWMFreq(PWM_freq_hMotor_ID id, PWM_freq newFrequency)
{
	uint8_t myPWM_pin;

	switch (id)
	{
#if BOARD(ROBOCORE)
	case PWM_freq_hMotor_ID_1_2: myPWM_pin = MYPWM_PIN_HMOT1; break;
	case PWM_freq_hMotor_ID_3:   myPWM_pin = MYPWM_PIN_HMOT3; break;
	case PWM_freq_hMotor_ID_4:   myPWM_pin = MYPWM_PIN_HMOT4; break;
	case PWM_freq_hMotor_ID_5:   myPWM_pin = MYPWM_PIN_HMOT5; break;
	case PWM_freq_hMotor_ID_6:   myPWM_pin = MYPWM_PIN_HMOT6; break;
#elif BOARD(CORE2)
	case PWM_freq_hMotor_ID_1:   myPWM_pin = MYPWM_PIN_HMOT1; break;
	case PWM_freq_hMotor_ID_2:   myPWM_pin = MYPWM_PIN_HMOT2; break;
	case PWM_freq_hMotor_ID_3:   myPWM_pin = MYPWM_PIN_HMOT3; break;
	case PWM_freq_hMotor_ID_4:   myPWM_pin = MYPWM_PIN_HMOT4; break;
#elif BOARD(CORE2MINI)
	case PWM_freq_hMotor_ID_1:   myPWM_pin = MYPWM_PIN_HMOT1; break;
	case PWM_freq_hMotor_ID_2:   myPWM_pin = MYPWM_PIN_HMOT2; break;
#else
#  error no board
#endif
	default: ASSERT(0);
	}
	myPWM_fix_freq_init(myPWM_pin, (pwm_freq_val_t)newFrequency);
	switch (id)
	{
#if BOARD(ROBOCORE)
	case PWM_freq_hMotor_ID_1_2:
		pimpls[0].updatePower_internal(true);
		pimpls[1].updatePower_internal(true);
		break;
	case PWM_freq_hMotor_ID_3: pimpls[2].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_4: pimpls[3].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_5: pimpls[4].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_6: pimpls[5].updatePower_internal(true); break;
#elif BOARD(CORE2)
	case PWM_freq_hMotor_ID_1: pimpls[0].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_2: pimpls[1].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_3: pimpls[2].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_4: pimpls[3].updatePower_internal(true); break;
#elif BOARD(CORE2MINI)
	case PWM_freq_hMotor_ID_1: pimpls[0].updatePower_internal(true); break;
	case PWM_freq_hMotor_ID_2: pimpls[1].updatePower_internal(true); break;
#else
#  error no board
#endif
	default: ASSERT(0);
	}
}


}
