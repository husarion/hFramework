/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_USB_H
#define H_USB_H

#include <hStreamDev.h>
#include <hMutex.h>
#include <hQueue.h>

namespace hFramework
{

/**
 * @brief Provides USB interface to Android Open Accessory
 */
class hUSB : public hStreamDev
{
public:
	hUSB(); //!< Initialize USB object.

	void init(); //!< Initialize USB connection.

	/**
	 * @brief Return if Android Open Accessory is connected.
	 * @return true, if AOA device is connected, false otherwise.
	 */
	bool isConnected();

	int write(const void* data, int len, uint32_t timeout = 5000);
	int read(void* data, int len, uint32_t timeout = 5000);

	/**
	 * @brief Return if data is available to read
	 * @return true, if data is available, false otherwise.
	 */
	bool isDataAvailable();

	//! @cond
	void task();

// private:
	hMutex m_mutex;
	hQueue<uint8_t> m_dataQueue;
	//! @endcond

private:
	void taskInternal();
	int writeInternal(uint8_t dest, const void* data, int len, uint32_t timeout = 5000);

	hUSB(const hUSB&) = delete;

	friend void sendInfoTask();
};

}

#endif /* H_USB_H */
