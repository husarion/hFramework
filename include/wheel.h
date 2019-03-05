#ifndef _WHEEL_H_
#define _WHEEL_H_

#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCyclicBuffer.h"

class Wheel {
  private:
	hMotor* mot;
	hPIDRegulator vReg;
	bool pol;

	int32_t lastPos[2];
	hCyclicBuffer<int32_t> lastPositions {hCyclicBuffer<int32_t>(2, lastPos)};

	float d0 = 0.0;
	float dNow = 0.0;

	float vTarget = 0.0;
	float vNow = 0.0;
	float vRange = 1000.0;
	float vSet = 0.0;
	float _max_speed = 0;
	float speed_step = 0;
	float vTarget_tmp = 0;

	bool turnedOn = 1;

  public:
	Wheel(hMotor &motor, bool polarity, float max_speed);
	void begin();

	/**
	 * @brief Trigger PID regulator update cycle.
	 * @param dt Regulator time step [ms]
	 */
	void update(uint32_t dt);

	/**
	 * @brief Set desired motor speed controlled by PID regulator
	 * @param speed Desired speed in encoder ticks per second
	 */
	void setSpeed(float speed);
	float getSpeed();
	
	int32_t getDistance();
	void resetDistance();

	void reset();
	void turnOff();
	void turnOn();
};

#endif
