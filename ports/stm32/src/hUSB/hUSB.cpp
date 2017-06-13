/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hTypes.h>

#if BOARD(ROBOCORE)

#include "hUSB.h"

#include <string.h>

extern "C"
{
#include "usb_bsp.h"
#include "usb_core.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_adk_core.h"
#include "usb_hcd_int.h"

	void OTG_FS_IRQHandler();
	void USB_OTG_BSP_DeInit();
}

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_Core_dev __ALIGN_END ;
__ALIGN_BEGIN USBH_HOST           USB_Host __ALIGN_END ;

extern __ALIGN_BEGIN ADK_Machine_TypeDef ADK_Machine __ALIGN_END;
extern __ALIGN_BEGIN USB_Setup_TypeDef   ADK_Setup __ALIGN_END;

#include "stm32f4xx.h"

#define USB_DEBUG
#define DEBUG_CATEGORY USB
#include "debug.h"

volatile int sent = 0, received = 0;

void OTG_FS_IRQHandler()
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

namespace hFramework
{

extern hUSB Usb;

hUSB::hUSB() : m_dataQueue(200)
{
}
void hUSB::init()
{
	USBH_Init(&USB_OTG_Core_dev,
	          USB_OTG_FS_CORE_ID,
	          &USB_Host,
	          &USBH_ADK_cb,
	          &USR_Callbacks);

	USBH_ADK_Init("Husarion", "RoboCORE", "RoboCORE Android", "1.0", "https://robocore.io/usb", "1234567");
}
bool hUSB::isConnected()
{
	return ADK_Machine.initstate == ADK_INIT_DONE;
}

int hUSB::write(const void* data, int len, uint32_t timeout)
{
	return writeInternal(0, data, len, timeout);
}
int hUSB::writeInternal(uint8_t dest, const void* data, int len, uint32_t timeout)
{
	if (!isConnected())
		return -2;

	URB_STATE URB_Status;
	// HC_STATUS HCD_Status;
	uint32_t startTime = sys.getRefTime();

	while (sys.getRefTime() - startTime < timeout)
	{
		if (m_mutex.take(10))
		{
			uint8_t dataTmp[len + 1];
			dataTmp[0] = dest;
			memcpy(dataTmp + 1, data, len);
			USBH_ADK_write(&USB_OTG_Core_dev, dataTmp, len + 1);

			URB_Status = HCD_GetURB_State(&USB_OTG_Core_dev, ADK_Machine.hc_num_out);
			// HCD_Status = HCD_GetHCState(&USB_OTG_Core_dev, ADK_Machine.hc_num_out);
			while (URB_Status == URB_IDLE)
			{
				USBH_Process(&USB_OTG_Core_dev, &USB_Host);
				if (timeout != 0 && sys.getRefTime() - startTime >= timeout)
				{
					m_mutex.give();
					return -1;
				}
				sys.delay(1);

				URB_Status = HCD_GetURB_State(&USB_OTG_Core_dev, ADK_Machine.hc_num_out);
				// HCD_Status = HCD_GetHCState(&USB_OTG_Core_dev, ADK_Machine.hc_num_out);
				// LOG("URB: %d HC: %d", URB_Status, HCD_Status);
			}

			m_mutex.give();
		}
		else
		{
			continue;
		}

		switch (URB_Status)
		{
		case URB_DONE:
			sent++;
			return len;
		case URB_NOTREADY:
			LOG("URB_NOTREADY");
			break;
		default:
			LOG("URB %d\r\n", URB_Status);
			return -1;
		}
	}
	return -1;
}
int hUSB::read(void* data, int len, uint32_t timeout)
{
	// if (!isConnected())
	// return -2;

	uint8_t* ptr = (uint8_t*)data;
	if (!m_dataQueue.receive(ptr[0], timeout))
		return 0;

	for (int i = 1; i < len; i++)
	{
		if (!m_dataQueue.receive(ptr[i], 0))
			return i;
	}
	return len;
}
bool hUSB::isDataAvailable()
{
	uint32_t xfercount;
	if (m_mutex.take(10))
	{
		xfercount = HCD_GetXferCnt(&USB_OTG_Core_dev, ADK_Machine.hc_num_in);
		m_mutex.give();
		return xfercount > 0;
	}
	else
	{
		return false;
	}
}

void hUSB::task()
{
	if (m_mutex.take(1))
	{
		taskInternal();
		m_mutex.give();
	}
}
void sendInfoTask()
{
	uint8_t data[1];
	data[0] = 2; // hFramework
	LOG("info bef sent");
	Usb.writeInternal(1, data, 1, 5000);
	LOG("info sent");
}
// void sendSessionInfoTask()
// {
// uint8_t data[1];
// data[0] = 2;
// LOG("info bef sent");
// Usb.writeInternal(1, data, 1, 5000);
// LOG("info sent");
// }
void hUSB::taskInternal()
{
	static int receiveRequestSent = 0;
	// static uint32_t receiveRequestTime = 0;
	// static int nakCnt = 0;
	// int timeout = 1000;

	USBH_Process(&USB_OTG_Core_dev, &USB_Host);

	// URB_STATE URB_Status;
	// HC_STATUS HCD_Status;

	uint32_t xfercount;
	uint8_t buf[USBH_ADK_DATA_SIZE];
	uint32_t i;

	if (isConnected())
	{
		if (receiveRequestSent)
		{
			// if (sys.getRefTime() - receiveRequestTime >= timeout)
			// {
			// receiveRequestSent = 0;
			// LOG("timeout1\r\n");
			// }
			// else
			{
				xfercount = HCD_GetXferCnt(&USB_OTG_Core_dev, ADK_Machine.hc_num_in);
				if (xfercount > 0)
				{
					USBH_ADK_read(&USB_OTG_Core_dev, buf, xfercount);

					received++;
					if (buf[0] == 0x00)
					{
						for (i = 1; i < xfercount; i++)
							m_dataQueue.sendToBack(buf[i], 0);
						LOG("queue tx %d", m_dataQueue.getElementCnt());
					}
					else if (buf[0] == 0x01)
					{
						LOG("ping received");
					}
					else if (buf[0] == 0x02)
					{
						LOG("sys.reset1");
						uint8_t time500ms = buf[1];

						NVIC_InitTypeDef NVIC_InitStructure;

						NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
						NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
						NVIC_Init(&NVIC_InitStructure);

						USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);

						sys.delay(60);

						USB_OTG_BSP_DeInit();

						sys.disableUsbCharging();
						sys.delay(6);
						sys.enableUsbCharging();
						sys.delay(6);

						sys.delay(500 * time500ms);

						LOG("sys.reset2");
						sys.reset();
					}
					else if (buf[0] == 0x03)
					{
						LOG("info requested");
						sys.taskCreate(sendInfoTask);
					}
					// 0x04 - bootloader
					// else if (buf[0] == 0x05)
					// {
					// LOG("sessinfo requested");
					// sys.taskCreate(sendSessionInfoTask);
					// }
					LOG("sent %d received %d", sent, received);

					receiveRequestSent = 0;
				}
			}
		}
		else
		{
			USBH_BulkReceiveData(&USB_OTG_Core_dev, ADK_Machine.inbuff, USBH_ADK_DATA_SIZE, ADK_Machine.hc_num_in);
			receiveRequestSent = 1;
			// receiveRequestTime = sys.getRefTime();
			// nakCnt = 0;
		}
	}
}

}

#endif
