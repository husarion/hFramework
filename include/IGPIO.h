/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __IGPIO_H__
#define __IGPIO_H__

#include <stdint.h>

#include <hTypes.h>

namespace hFramework
{

/**
 * @brief Interface for General Purpose Input-Output pin (GPIO).
 */
class IGPIO
{
public:
	virtual void setOut() = 0; //!< Set GPIO to output push-pull mode.
	virtual void setOutOD() = 0; //!< Set GPIO to output open-drain mode.
	virtual void setOutOD_pu() = 0; //!< Set GPIO to output open-drain mode with pull-up resistor.
	virtual void setOutOD_pd() = 0; //!< Set GPIO to output open-drain mode with pull-down resistor.
	virtual void setIn() = 0; //!< Set GPIO to input.
	virtual void setIn_pu() = 0; //!< Set GPIO to input mode with pull-up resistor.
	virtual void setIn_pd() = 0; //!< Set GPIO to input mode with pull-down resistor.

	/**
	 * @brief Read GPIO state.
	 * @return true, if high level (~3.3V) is present on current GPIO, otherwise false when low level (~0V)
	 */
	virtual bool read() = 0;

	/**
	 * @brief Set GPIO to high or low voltage level.
	 * @param value true, to set high level (~3.3V) or false to set low level (~0V)
	 */
	virtual void write(bool value) = 0;

	virtual void toggle() = 0; //!< Set GPIO to opposite voltage level
	virtual void reset() = 0; //!< Set GPIO to default state (input mode).
};

/**
 * @brief Interface for General Purpose Input-Output pin (GPIO) with interrupt capability.
 */
class IGPIO_int : public virtual IGPIO
{
public:

	/**
	 * @brief Disable interrupt on the current pin.
	 */
	virtual void interruptOff() = 0;

	virtual void interruptOn(InterruptEdge edge, CountingMode mode = CountingMode::Normal) { _interruptOn(edge, mode, doNothing); }
	virtual void interruptOn(InterruptEdge edge, CountingMode mode, const std::function<void()>& handler) { _interruptOn(edge, mode, handler); }
	/*__attribute__((deprecated))*/ virtual void interruptOn(InterruptEdge edge, CountingMode mode, UserDataFunc handler, void* userdata) { _interruptOn(edge, mode, std::bind(handler, userdata)); }
	virtual void interruptOn(InterruptEdge edge, const std::function<void()>& handler) { _interruptOn(edge, CountingMode::Normal, handler); }

	/**
	 * @brief Enable external interrupt on the current pin when falling edge occurs.
	 * @param mode interrupt mode:
	 * - 0: only one interrupt is remembered
	 * - 1: remember many interrupts (if you want to recognise all interrupts,
	 *      even if interval between them is too small to serve all)
	 * @param callback pointer to function to be executed in case of interrupt
	 */
	void interruptOn_EdgeFalling(CountingMode mode = CountingMode::Normal) { interruptOn(InterruptEdge::Falling, mode); }
	void interruptOn_EdgeFalling(CountingMode mode, const std::function<void()>& handler) { interruptOn(InterruptEdge::Falling, mode, handler); }
	__attribute__((deprecated)) void interruptOn_EdgeFalling(CountingMode mode, UserDataFunc handler, void* userdata) { interruptOn(InterruptEdge::Falling, mode, handler, userdata); }

	/**
	 * @brief Enable external interrupt on the current pin when rising edge occurs.
	 * @param mode interrupt mode:
	 * - 0: only one interrupt is remembered
	 * - 1: remember many interrupts (if you want to recognise all interrupts,
	 *      even if interval between them is too small to serve all)
	 * @param callback pointer to function to be executed in case of interrupt
	 */
	void interruptOn_EdgeRising(CountingMode mode = CountingMode::Normal) { interruptOn(InterruptEdge::Rising, mode); }
	void interruptOn_EdgeRising(CountingMode mode, const std::function<void()>& handler) { interruptOn(InterruptEdge::Rising, mode, handler); }
	__attribute__((deprecated)) void interruptOn_EdgeRising(CountingMode mode, UserDataFunc handler, void* userdata) { interruptOn(InterruptEdge::Rising, mode, handler, userdata); }

	/**
	 * @brief Enable external interrupt on the current pin when falling or rising edge occurs.
	 * @param mode interrupt mode:
	 * - 0: only one interrupt is remembered
	 * - 1: remember many interrupts (if you want to recognise all interrupts,
	 *      even if interval between them is too small to serve all)
	 * @param callback pointer to function to be executed in case of interrupt
	 */
	void interruptOn_EdgeChange(CountingMode mode = CountingMode::Normal) { interruptOn(InterruptEdge::Both, mode); }
	void interruptOn_EdgeChange(CountingMode mode, const std::function<void()>& handler) { interruptOn(InterruptEdge::Both, mode, handler); }
	__attribute__((deprecated)) void interruptOn_EdgeChange(CountingMode mode, UserDataFunc& handler, void* userdata) { interruptOn(InterruptEdge::Both, mode, handler, userdata); }

	virtual void interruptSetEdge(InterruptEdge edge) = 0;

	/**
	 * @brief Wait for external interrupt.
	 * @param timeout time (in ms) to wait for external interrupt
	 * @return true if interrupt occurs before timeout, false otherwise.
	 */
	virtual bool interruptWait(uint32_t timeout = 0xffffffff) = 0;

protected:
	virtual void _interruptOn(InterruptEdge edge, CountingMode mode, const std::function<void()>& handler) = 0;
};

/**
 * @brief Interface for General Purpose Input-Output pin (GPIO) with ADC capability.
 */
class IGPIO_adc : public virtual IGPIO
{
public:
	virtual void enableADC() = 0; //!< Enable Analog to Digital converter on the current pin
	virtual void disableADC() = 0; //!< Disable Analog to Digital converter on the current pin

	/**
	 * @brief Read analog value on the current pin.
	 * @return voltage (in [V]) on the current pin.
	 */
	virtual float analogReadVoltage() = 0;

	/**
	 * @brief Read raw analog value on the current pin.
	 * @return decimal value in range from 0 to 4095 that corresponds to the current GPIO input voltage.
	 */
	virtual uint16_t analogReadRaw() = 0;
};

/**
 * @brief Interface for General Purpose Input-Output pin (GPIO) with interrupt and ADC capability.
 */
class IGPIO_super : public virtual IGPIO_int, public virtual IGPIO_adc
{
public:
};

}
#endif
