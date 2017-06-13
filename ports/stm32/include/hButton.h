/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "hTypes.h"
#include "hGPIO.h"

namespace hFramework
{

typedef enum
{
#if BOARD(ROBOCORE)
	hButton_ID_SW1,
	hButton_ID_SW2,
#elif BOARD(CORE2)
	hButton_ID_SW1,
	hButton_ID_SW2,
	hButton_ID_SW3,
#elif BOARD(CORE2MINI)
	hButton_ID_SW1,
	hButton_ID_SW3,
#else
#  error no board
#endif
} hButton_ID;

/**
 * @brief Provides interface to on-board buttons.
 *
 * Available global instances: hBtn1, hBtn2
 */
class hButton_int
{
	// @cond
	friend class hSystem;
	// @endcond

public:
	// @cond
	hButton_int(hButton_ID id);
	// @endcond

	/**
	 * @brief Check if button is pressed.
	 * @return true, if button is pressed, false otherwise
	 */
	bool isPressed();

	/**
	 * @brief Set handler to be executed on button press.
	 * @param handler handler
	 * @param userdata data passed as argument when handler is invoked
	 */
	void setOnPressHandler(const std::function<void()>& handler) { pressHandler = handler; }

	/**
	 * @brief Set handler to be executed on button press.
	 * @param handler handler
	 * @param userdata data passed as argument when handler is invoked
	 */
	__attribute__((deprecated))
	void setOnPressHandler(UserDataFunc handler, void* userdata) { releaseHandler = std::bind(handler, userdata); }

	/**
	 * @brief Set handler to be executed on button release.
	 * @param handler handler
	 * @param userdata data passed as argument when handler is invoked
	 */
	__attribute__((deprecated))
	void setOnReleaseHandler(const std::function<void()>& handler) { releaseHandler = handler; }

	/**
	 * @brief Set handler to be executed on button release.
	 * @param handler handler
	 * @param userdata data passed as argument when handler is invoked
	 */
	void setOnReleaseHandler(UserDataFunc handler, void* userdata) { releaseHandler = std::bind(handler, userdata); }

	/**
	 * @brief Wait for button press event.
	 * @param timeout timeout in milliseconds, INFINITE to wait forever
	 * @return true, if button was pressed, false if timeout occured.
	 */
	bool waitForPressed(uint32_t timeout = INFINITE);

	/**
	 * @brief Wait for button release event.
	 * @param timeout timeout in milliseconds, INFINITE to wait forever
	 * @return true, if button was released, false if timeout occured.
	 */
	bool waitForReleased(uint32_t timeout = INFINITE);

private:
	hGPIO_int pin;
	std::function<void()> pressHandler, releaseHandler;
	void *timer;

	void init();

	static void userHandler(void* timer);
};

class hButton
{
	// @cond
	friend class hSystem;
	// @endcond

public:
	// @cond
	hButton(hButton_ID id);
	// @endcond

	/**
	 * @brief Check if button is pressed.
	 * @return true, if button is pressed, false otherwise
	 */
	bool isPressed();

private:
	hGPIO pin;

	void init();
};

}

#endif
