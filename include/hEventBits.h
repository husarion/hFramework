/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_EVENT_BITS_H
#define H_EVENT_BITS_H

#include "hTypes.h"

namespace hFramework
{

/**
 * @brief Manipulates eventBits structure.
 *
 * One object can hold up to 24 bits (8 highest bits are discarded).
 * Event bits are used to wait for one or multiple flags, without wasting CPU.
 */
class hEventBits
{
public:
	/**
	 * @brief Initialize object.
	 */
	hEventBits();
	/**
	 * @brief Destroy object.
	 */
	~hEventBits();
	/**
	 * @brief Wait for all masked bits to be set.
	 * @param bitMask mask of the bits to be waiting for (1 in position of active bit)
	 * @param timeout time (in ms) to wait for bits to be set
	 * @return bits that are set in EventBits object. Before timeout return value should correspond to "bitMask".
	 * @code
	 * hEventBits eb;
	 * void hMain() {
	 *   while(1) {
	 *     eb.waitAll(0x0080AA01); // RTOS blocks current task until ALL bits: 0, 9, 11, 13, 15, 23 are set
	 *     // ... do some things
	 *   }
	 * }
	 * @endcode
	 */
	uint32_t waitAll(uint32_t bitMask, uint32_t timeout = 0xffffffff);

	/**
	 * @brief Wait for any masked bit to be set.
	 * @param bitMask mask of the bits to be waiting for
	 * @param timeout time (in ms) to wait for bits to be set
	 * @return bits that are set in EventBits object.
	 * @code
	 * hEventBits eb;
	 * void hMain() {
	 *   while(1) {
	 *     eb.waitAny(0x0080AA01); // RTOS blocks current task until ANY of the bits: 0, 9, 11, 13, 15, 23 is set
	 *     // ... do some things
	 *   }
	 * }
	 * @endcode
	 */
	uint32_t waitAny(uint32_t bitMask, uint32_t timeout = 0xffffffff);

	/**
	 * @brief Set bits in the object.
	 * @param bitsToSet bits to be set
	 * @return bits that are set in EventBits object after setting bits.
	 */
	uint32_t setBits(uint32_t bitsToSet);

	/**
	 * @brief Clear bits in the object.
	 * @param bits bits to be cleared
	 * @return bits that are set in EventBits object before clearing bits.
	 */
	uint32_t clearBits(uint32_t bitsToClear);

	/**
	 * @brief Clear all bits in the object
	 * @return bits that are set in EventBits object before clearing bits.
	 */
	uint32_t clear();

	/**
	 * @brief Return bits that are set inEventBits object
	 * @return bits that are set inEventBits object.
	 */
	uint32_t get();

private:
	class hEventBitsImplementation;
	hEventBitsImplementation * impl;

	hEventBits(const hEventBits&) = delete;
};

}

#endif /* H_EVENT_BITS_H */
