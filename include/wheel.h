#ifndef _WHEEL_H_
#define _WHEEL_H_

#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCyclicBuffer.h"

/** A class for controlling single wheel.
 *  Class is using PID regulator for controlling wheel speed.
 */
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
	
	bool turnedOn = 1;
	
public:
	/** 
	 * Default constructor.
	 * @param motor Pointer to motor which should be controlled.
	 * @param polarity Determines which way is the positive rotation direction, if true, the direction is reversed.
	 */
	Wheel(hMotor& motor, bool polarity);
	void begin();

	/**
	 * @brief Trigger PID regulator update cycle.
	 * @param dt Regulator time step [ms]
	 */
	void update(uint32_t dt);
	
	/**
	 * @brief Set desired motor speed controlled by PID regulator.
	 * @param speed Desired speed in encoder ticks per second
	 */
	void setSpeed(float speed);

	/**
	 * @brief Get current wheel speed.
	 * @return Current speed in encoder ticks per second
	 */
	float getSpeed();
	
	/**
	 * @brief Get distance travelled by the wheel.
	 * @return Distance in encoder ticks
	 */
	int32_t getDistance();

	/**
	 * @brief Reset wheel to initial state.
	 */	
	void reset();

	/**
	 * @brief Disable wheel operation.
	 */
	void turnOff();

	/**
	 * @brief Enable wheel operation.
	 */
	void turnOn();
};

#endif
