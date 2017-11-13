/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hMotor.h"
#include "hSystem.h"
#include "hTypes.h"
#include "hQueue.h"
#include "hMutex.h"
#include <IServo.h>

#include "myEncoder.h"
#include "myGPIO.h"
#include "myPWM.h"
#include "hGPIO.h"
#include "FreeRTOS.h"

#include "hUtils.h"
#define HMOTOR_DEBUG
#define DEBUG_CATEGORY HMOTOR
#include "debug.h"
#include "ycm.h"

#include "hMotorPimpl.h"

namespace hFramework
{

// hServo_hMotor
void hServo_hMotor::init()
{
	int pinPWM = hMotorDefs[impl.motorId].pinPWM;
	myPWM_setCnt_ns(pinPWM, 0);
	myPWM_setPeriod_us(pinPWM, 20000);
	myPWM_setWidth_us(pinPWM, 0);
	myPWM_counter_enable(pinPWM);
}
void hServo_hMotor::setWidth(uint16_t width_us)
{
	if (impl.mode != hMotorPimpl::SERVO)
		return;
	this->curWidth = width_us;

	int pinPWM = hMotorDefs[impl.motorId].pinPWM;
	myPWM_setWidth_us(pinPWM, width_us);
}
void hServo_hMotor::setPeriod(uint16_t period_us)
{
	if (impl.mode != hMotorPimpl::SERVO)
		return;

	int pinPWM = hMotorDefs[impl.motorId].pinPWM;
	myPWM_setPeriod_us(pinPWM, period_us);
}

// hMotor
hMotor::hMotor(hMotor_ID id)
	: impl(pimpls[id])
{
	impl.initialized = false;
	impl.motorId = id;
}

bool hMotor::init()
{
	return impl.init();
}

void hMotor::setPower(int16_t power)
{
	impl.setPower(power);
}
void hMotor::setPowerLimit(int16_t limit)
{
	impl.limit = limit;
	impl.updatePower_internal(false);
}
void hMotor::setSlewRate(float time)
{
	impl.setSlewRate(time);
}

void hMotor::attachPositionRegulator(hRegulator& regPosition)
{
	impl.attachPositionRegulator(regPosition);
}

bool hMotor::rotAbs(int32_t angle, uint16_t power, bool block, uint32_t timeout)
{
	return impl.rotAbs(angle, power, block ? timeout : 0);
}
bool hMotor::rotRel(int32_t angle, uint16_t power, bool block, uint32_t timeout)
{
	return impl.rotRel(angle, power, block ? timeout : 0);
}
bool hMotor::calibrate(int16_t power, uint32_t maxDiff, uint32_t stableTime, bool block, uint32_t timeout)
{
	return impl.calibrate(power, maxDiff, stableTime, block ? timeout : 0);
}

bool hMotor::stopRegulation(bool block, uint32_t timeout)
{
	return impl.stopRegulation(block, timeout);
}

bool hMotor::waitDone(uint32_t timeout)
{
	return impl.waitDone(timeout);
}

int32_t hMotor::getEncoderCnt(void)
{
	return myEncoder_getCnt(hMotorDefs[impl.motorId].encoderNr);
}

void hMotor::resetEncoderCnt(void)
{
	return myEncoder_resetCnt(hMotorDefs[impl.motorId].encoderNr);
}

void hMotor::setParallelMode()
{
	impl.setParallelMode();
}
void hMotor::setSingleMode()
{
	impl.setSingleMode();
}

void hMotor::setActiveBreaking()
{
	impl.setActiveBreaking();
}

void hMotor::setPassiveBreaking()
{
	impl.setPassiveBreaking();
}

void hMotor::setMotorPolarity(Polarity polarity)
{
	impl.motorPolarity = polarity;
	impl.updatePower_internal(true);
}
void hMotor::setEncoderPolarity(Polarity polarity)
{
	impl.encoderPolarity = polarity;
	portDISABLE_INTERRUPTS();
	myEncoder_setPolarity(hMotorDefs[impl.motorId].encoderNr, polarity == Polarity::Normal ? 0 : 1);
	portENABLE_INTERRUPTS();
}
void hMotor::setEncoderPu(){
	impl->setPullup();
}
void hMotor::setEncoderPd(){
	impl->setPulldown();
}
void hMotor::setEncoderHI(){
	impl->setPullReset();
}
Polarity hMotor::getMotorPolarity() const
{
	return impl.motorPolarity;
}
Polarity hMotor::getEncoderPolarity() const
{
	return impl.encoderPolarity;
}

IServo& hMotor::useAsServo()
{
	int pinA = hMotorDefs[impl.motorId].pinMotA;
	int pinB = hMotorDefs[impl.motorId].pinMotB;

	myGPIO_dir_out(pinB);
	myGPIO_write(pinB, 1);
	myGPIO_dir_in(pinA);

	impl.mode = hMotorPimpl::SERVO;
	impl.servo.init();
	return impl.servo;
}
void hMotor::releaseServo()
{
	impl.mode = hMotorPimpl::MOTOR;
}

void hMotor::setPWMFreq(PWM_freq_hMotor_ID id, PWM_freq newFrequency)
{
	hMotorPimpl::setPWMFreq(id, newFrequency);
}

} /* namespace hFramework */
