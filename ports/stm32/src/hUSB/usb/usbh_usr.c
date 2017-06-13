/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team and modified by Yuuichi Akagawa
  * @version V2.0.0
  * @date    22-July-2011, 2012/03/05
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"

#include "stm32f4xx.h"

#define DEBUG_CATEGORY USB
#include "debug.h"

#include "usbh_adk_core.h"

extern USB_OTG_CORE_HANDLE      USB_OTG_Core_dev;
extern USBH_HOST                USB_Host;

USBH_Usr_cb_TypeDef USR_Callbacks =
{
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_ADK_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};

void USBH_USR_Init(void)
{
	static uint8_t startup = 0;
	if (startup == 0)
	{
		startup = 1;
		LOG("Host Library Initialized");
	}
}

void USBH_USR_DeviceAttached(void)
{
	LOG("Device Attached");
}

void USBH_USR_UnrecoveredError(void)
{
	LOG("UNRECOVERED ERROR STATE");
}

void USBH_USR_DeviceDisconnected(void)
{
	LOG("Device Disconnected");
}

void USBH_USR_ResetDevice(void)
{
	LOG("USBH_USR_ResetDevice");
	/* Users can do their application actions here for the USB-Reset */
}

void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
	{
		LOG("High speed device detected");
	}
	else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
	{
		LOG("Full speed device detected");
	}
	else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
	{
		LOG("Low speed device detected");
	}
	else
	{
		LOG("Device fault");
	}
}

void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs;
	hs = DeviceDesc;
	
	LOG("Device Descriptor");
	LOG("Class : %02Xh", (uint8_t)(*hs).bDeviceClass);
	LOG("SubClass : %02Xh", (uint8_t)(*hs).bDeviceSubClass);
	LOG("Protocol : %02Xh", (uint8_t)(*hs).bDeviceProtocol);
	LOG("VID : %04Xh", (uint32_t)(*hs).idVendor);
	LOG("PID : %04Xh", (uint32_t)(*hs).idProduct);
}

void USBH_USR_DeviceAddressAssigned(void)
{
	LOG("Address assigned");
}

void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id;
	id = itfDesc;
	
	LOG("Interface : %02X %02X", (uint8_t)(*id).bInterfaceClass, (uint8_t)(*id).bInterfaceSubClass);
	LOG("Power info: bmAttributes:0x%02X, bMaxPower:%d", cfgDesc->bmAttributes, cfgDesc->bMaxPower);
}

void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	LOG("Manufacturer : %s", (char *)ManufacturerString);
}

void USBH_USR_Product_String(void *ProductString)
{
	LOG("Product : %s", (char *)ProductString);
}

void USBH_USR_SerialNum_String(void *SerialNumString)
{
	LOG("Serial Number : %s", (char *)SerialNumString);
}

void USBH_USR_EnumerationDone(void)
{
	LOG("Enumeration completed");
}

void USBH_USR_DeviceNotSupported(void)
{
	LOG("Device not supported.");
}

USBH_USR_Status USBH_USR_UserInput(void)
{
	LOG("USBH_USR_UserInput");
	return USBH_USR_RESP_OK;
}

void USBH_USR_OverCurrentDetected(void)
{
	LOG("Overcurrent detected.");
}

void USBH_USR_DeInit(void)
{
	LOG("USBH_USR_DeInit");
	USBH_Init(&USB_OTG_Core_dev,
	          USB_OTG_FS_CORE_ID,
	          &USB_Host,
	          &USBH_ADK_cb,
	          &USR_Callbacks);
}

int USBH_USR_ADK_Application(void)
{
	LOG("USBH_USR_ADK_Application");
	return 0;
}
