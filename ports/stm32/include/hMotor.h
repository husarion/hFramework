/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_MOTOR_H_
#define H_MOTOR_H_

#include "hTypes.h"
#include "IServo.h"
#include "hRegulator.h"

namespace hFramework
{

/*!
*  \addtogroup hFramework
*  @{
*/
/**
 * @brief Motor port ID.
 */
enum hMotor_ID
{
#if BOARD(ROBOCORE)
	hMotor_ID_1, //!< Motor 1
	hMotor_ID_2, //!< Motor 2
	hMotor_ID_3, //!< Motor 3
	hMotor_ID_4, //!< Motor 4
	hMotor_ID_5, //!< Motor 5
	hMotor_ID_6, //!< Motor 6
#elif BOARD(CORE2)
	hMotor_ID_1, //!< Motor 1
	hMotor_ID_2, //!< Motor 2
	hMotor_ID_3, //!< Motor 3
	hMotor_ID_4, //!< Motor 4
#elif BOARD(CORE2MINI)
	hMotor_ID_1, //!< Motor 1
	hMotor_ID_2, //!< Motor 2
#else
#  error no board
#endif
};

/**
 * @brief Motor ID for PWM frequency settings.
 */
enum PWM_freq_hMotor_ID
{
#if BOARD(ROBOCORE)
	PWM_freq_hMotor_ID_1_2, //!< Motors 1 and 2
	PWM_freq_hMotor_ID_3,   //!< Motor 3
	PWM_freq_hMotor_ID_4,   //!< Motor 4
	PWM_freq_hMotor_ID_5,   //!< Motor 5
	PWM_freq_hMotor_ID_6,   //!< Motor 6
#elif BOARD(CORE2)
	PWM_freq_hMotor_ID_1,   //!< Motor 1
	PWM_freq_hMotor_ID_2,   //!< Motor 2
	PWM_freq_hMotor_ID_3,   //!< Motor 3
	PWM_freq_hMotor_ID_4,   //!< Motor 4
#elif BOARD(CORE2MINI)
	PWM_freq_hMotor_ID_1,   //!< Motor 1
	PWM_freq_hMotor_ID_2,   //!< Motor 2
#else
#  error no board
#endif
};

/**
 * @brief Avaiable values of PWM frequency for hMotor.
 */
enum PWM_freq
{
	PWM_freq_21_kHz = 0, //!< 21 kHz
	PWM_freq_12_kHz,     //!< 12 kHz
	PWM_freq_6_kHz,      //!< 6 kHz
	PWM_freq_4_kHz,      //!< 4 kHz
	PWM_freq_3_kHz,      //!< 3 kHz
	PWM_freq_2_kHz,      //!< 2 kHz
	PWM_freq_1_kHz,      //!< 1 kHz
	PWM_freq_500_Hz,     //!< 500 Hz
	PWM_freq_300_Hz,     //!< 300 Hz
	PWM_freq_200_Hz,     //!< 200 Hz
	PWM_freq_100_Hz,     //!< 100 Hz
	PWM_freq_50_Hz,      //!< 50 Hz
	PWM_freq_25_Hz,      //!< 25 Hz
	PWM_freq_10_Hz,      //!< 10 Hz
	PWM_freq_5_Hz,       //!< 5 Hz
	PWM_freq_1_Hz        //!< 1 Hz
};
/*! @} */

class hMotorPimpl;
/**
 * @brief Provides control of on-board motor driver.
 */
class hMotor
{
	friend class hSystem;

public:
	/**
	 * @brief Initialize specific motor driver. For internal use only - should never be called by user.
	 * @param id motor id
	 */
	hMotor(hMotor_ID id);

	/**
	 * @brief change default PWM frequency for a specific motor driver.
	 * @param id motor id
	 * @param newFrequency new PWM frequency value
	 */
	static void setPWMFreq(PWM_freq_hMotor_ID id, PWM_freq newFrequency);

	/**
	 * @brief Replace default position regulator (for "rotX" methods).
	 * @param regPosition a reference to a position regulator object.
	 */
	void attachPositionRegulator(hRegulator& regPosition);

	/**
	 * @brief Rotate "angle" encoder ticks starting from a motor position at the beginning (when you start using "rotX" functions).
	 * @param angle number of encoder ticks to rotate (negative value - left, positive value - right)
	 * @param power a power that is attached to a motor, the higher the value, the shorter time to reach final position (values from 0 to 1000).
	 * @param block 1 - if you want to block the task until final angle is reached (waiting for maximum time specified by timeout argument)
	 * @param timeout 0 - if you don't want to block, any other value is the maximum time to block the task until final angle is reached.
	 * @return 1 if order has been successfully stored in motor queue
	 */
	bool rotAbs(int32_t angle, uint16_t power = 1000, bool block = false, uint32_t timeout = INFINITE);

	/**
	 * @brief Rotate "angle" encoder ticks starting from a current motor position
	 * @param angle number of encoder ticks to rotate (negative value - left, positive value - right)
	 * @param power a power that is attached to a motor, the higher the value, the shorter time to reach final position (values from 0 to 1000).
	 * @param block 1 - if you want to block the task until final angle is reached (waiting for maximum time specified by timeout argument)
	 * @param timeout 0 - if you don't want to block, any other value is the maximum time to block the task until final angle is reached.
	 * @return 1 if order has been successfully stored in motor queue
	 */
	bool rotRel(int32_t angle, uint16_t power = 1000, bool block = false, uint32_t timeout = INFINITE);

	/**
	 */
	bool calibrate(int16_t power, uint32_t maxDiff, uint32_t stableTime, bool block = false, uint32_t timeout = INFINITE);

	/**
	 * @brief Set a power attached to a motor.
	 * @param power a power that is attached to a motor, the higher the value, the more power is attached to a motor 
	 * (values from -1000 to 1000). The positive and negative values correspond to normal or reversed polarity of the motor. 
	 * In other words, this value is proportional to the the average voltage between the A and B outputs of H-bridge driver (can be positive or negative).
	 * @return 1 if order has been successfully stored in motor queue
	 */
	void setPower(int16_t power);

	void setPowerLimit(int16_t limit);
	
	
	/**
	* @brief Set slew rate for motor
	* @param time (min 0.01) Time constant of rate regulation 
	*/
	void setSlewRate(float time);

	/**
	 * @brief Get number of encoder ticks.
	 * @return encoder ticks
	 */
	int32_t getEncoderCnt();

	/**
	 * @brief Set encoder ticks to 0
	 */
	void resetEncoderCnt();

	/**
	 * @brief Stop the motor
	 */
	void stop() { setPower(0); }

	/**
	 * @brief Wait until all tasks are finished by the motor.
	 * @param timeout time to wait for a motor to done all operations.
	 * @return 1 if motor has finished all its operations from its queue before timeout.
	 */
	bool waitDone(uint32_t timeout = INFINITE);

	/**
	 * @brief Get a current motor index.
	 * @return 1 - hMotor1, 2 - hMotor2, ... , 6 - hMotor6
	 */
	uint8_t getMotorNr(void);

	/**
	 * @brief Stop motor regulation
	 */
	bool stopRegulation(bool block = false, uint32_t timeout = INFINITE);

	/**
	 * @brief Set parallel mode. Motors are controlled in pairs of two.
	 *
	 * Since call of this function, two motor drivers
	 * are used as one in the following manner:
	 * - motor 1 is used with motor 2
	 * - motor 3 is used with motor 4
	 * - motor 5 is used with motor 6
	 */
	void setParallelMode();

	/**
	 * @brief Return to single mode. Each motor is controlled independently from each other.
	 */
	void setSingleMode();

	void setActiveBreaking();

	void setPassiveBreaking();

	/**
	 * @brief Set motor polarity
	 * @param polarity motor polarity
	 */
	void setMotorPolarity(Polarity polarity);

	/**
	 * @brief Set encoder polarity
	 * @param polarity encoder polarity
	 */
	void setEncoderPolarity(Polarity polarity);
	
	/**
	* @brief Set encoder pin A and B as pullups
	*/
	void setEncoderPu();
	
	/**
	* @brief Set encoder pin A and B as pulldownss
	*/
	void setEncoderPd();
	
	/**
	* @brief set encoder pin A and B as floating
	*/
	void setEncoderHZ();

	/**
	 * @brief Return motor polarity.
	 * @return motor polarity
	 */
	Polarity getMotorPolarity() const;

	/**
	 * @brief Return encoder polarity.
	 * @return encoder polarity
	 */
	Polarity getEncoderPolarity() const;

	/**
	 * @brief Configure hMotor as servo and return Servo object.
	 * @return servo object
	 */
	IServo& useAsServo();

	/**
	 * @brief Release servo object and allow to use hMotor with motor again.
	 */
	void releaseServo();

private:
	hMotorPimpl& impl;

	hMotor(const hMotor&) = delete;

	bool init();
};

}

#endif	// H_MOTOR
