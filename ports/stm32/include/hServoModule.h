/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HSERVOMODULE_H__
#define __HSERVOMODULE_H__

#include <hTypes.h>

#ifdef SERVOMODULE_ENABLED

#include <stdint.h>

#include <IServo.h>
#include <hMutex.h>

namespace hFramework
{

class hServoModule_Servo : public IServo
{
public:
	hServoModule_Servo(int num);

	void setWidth(uint16_t widthUs);
	void setPeriod(uint16_t periodUs);

private:
	int num;

	hServoModule_Servo(const hServoModule_Servo&) = delete;
};

class hServoModuleClass
{
public:
	hServoModuleClass();

	void setWidth(int num, uint16_t widthUs);
	void setPeriod(uint16_t periodUs);

	void enablePower();
	void disablePower();

#if BOARD(CORE2)
	//! Set power line voltage for all servos to about 5 [V].
	void setVoltage5V() { setPowerMode(0); }
	//! Set power line voltage for all servos to about 6 [V].
	void setVoltage6V() { setPowerMode(1); }
	//! Set power line voltage for all servos to about 7.4 [V].
	void setVoltage7V4() { setPowerMode(2); }
	//! Set power line voltage for all servos to about 8.6 [V].
	void setVoltage8V6() { setPowerMode(3); }
#endif

	float getVoltage();
	uint32_t getVoltageMV();

	hServoModule_Servo& getServo(int num);

#if BOARD(CORE2)
	hServoModule_Servo servo1, servo2, servo3, servo4, servo5, servo6;
#elif BOARD(CORE2MINI)
	hServoModule_Servo servo1, servo2, servo3, servo4;
#else
#  error no board
#endif

private:
#if BOARD(CORE2)
	void setPowerMode(int mode);
#endif

	static void init();

	hServoModuleClass(const hServoModuleClass&) = delete;

	friend class hSystem;
};

}

#endif

#endif
