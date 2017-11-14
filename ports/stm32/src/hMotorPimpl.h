/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HMOTORPIMPL_H__
#define __HMOTORPIMPL_H__

#include <cstdint>

#include <IServo.h>
#include <hMotor.h>
#include <hQueue.h>
#include <hMutex.h>

#include <port.h>

namespace hFramework
{

#define DRIVER_MODE_NONE     0
#define DRIVER_MODE_SINGLE   1
#define DRIVER_MODE_PARALLEL 2

struct TMotorDef
{
	uint8_t pinMotA, pinMotB, pinPWM, encoderNr;
	uint8_t driverId;
};

extern const TMotorDef hMotorDefs[];

struct TDriverDef
{
	uint8_t pinSleep, pinModeA, pinModeB;
	uint8_t slaveMotorId;
	uint8_t controlMotorId; /* which motor should control the driver in parallel mode (this one
                             drives PWM) */
};

extern const TDriverDef hDriverDefs[];

extern uint8_t driverModes[];

struct TMotorOperation
{
	enum class Op : uint8_t { STOP_REGULATOR, ROTATE, CALIBRATE };
	enum class Flags : uint8_t { NONE = 0, ABSOLUTE = 0x04, RELATIVE = 0x08 };

	Op op;
	Flags flags;
	union
	{
		struct
		{
			uint32_t angle;
			uint16_t power;
		} rot;
		struct
		{
			uint32_t maxDiff, stableTime;
			int16_t power;
		} calib;
	} param;
	hRegulator* regulator;
};

class hMotorPimpl;
class hServo_hMotor : public virtual IServo
{
public:
	hServo_hMotor(hMotorPimpl& pimpl) : impl(pimpl) { }

	void init();

	void setWidth(uint16_t width_us);
	void setPeriod(uint16_t period_us);

private:
	hMotorPimpl& impl;
};

class hMotorPimpl
{
public:
	enum EMode { MOTOR, SERVO };

	hMotorPimpl() : servo(*this) { }

	EMode mode;
	hRecursiveMutex accessMutex;
	hRecursiveMutex setPowerMutex;
	bool activeBreaking;

	int motorId, controlMotorId;
	bool initialized;
	Polarity motorPolarity;
	Polarity encoderPolarity;
	int16_t curPower, limit;

	hServo_hMotor servo;

	// regulation
	hRegulator* regPosition;
	hSemaphore operationDoneMutex;
	bool regulationDone;
	hTask *regTask;
	bool regTaskRunning;
	volatile int32_t regTargetEncoder;
	volatile uint16_t regPower;
	volatile int regMode;
	volatile bool regTaskExit;

	// slew rate
	hTask *slewTask;
	bool slewTaskRunning;
	volatile bool slewTaskExit;
	volatile uint32_t slewRate;
	volatile int16_t slewTarget;

	bool init();
	void setPower(int16_t power);
	void updatePower_internal(bool force) { updatePower_internal(force, curPower); }
	void updatePower_internal(bool force, int16_t power);
	void setSlewRate(float time);
	void slewRateTask();

	void attachPositionRegulator(hRegulator& regPosition);
	bool rotAbs(int32_t angle, uint16_t power, uint32_t timeout);
	bool rotRel(int32_t angle, uint16_t power, uint32_t timeout);
	bool calibrate(int16_t power, uint32_t maxDiff, uint32_t stableTime, uint32_t timeout);
	bool doRotate(uint32_t timeout);
	bool waitDone(uint32_t timeout);
	bool stopRegulation(bool block, uint32_t timeout);
	void regulatorTask();

	void setParallelMode();
	void setSingleMode();
	void setActiveBreaking() { activeBreaking = true; updatePower_internal(true); }
	void setPassiveBreaking() { activeBreaking = false; updatePower_internal(true); }
	
	void setPullup();
	void setPulldown();
	void setPullReset();

	int getDriverId() const
	{
		return hMotorDefs[motorId].driverId;
	}
	const TDriverDef& getDriver() const
	{
		return hDriverDefs[getDriverId()];
	}
	int getDriverMode() const
	{
		return driverModes[getDriverId()];
	}

	static void setupDrivers();
	static void setDefaults();
	static void setPWMFreq(PWM_freq_hMotor_ID id, PWM_freq newFrequency);

	friend class hMotor;
};

extern hMotorPimpl pimpls[ROBOCORE_HMOTOR_CNT];

}

#endif
