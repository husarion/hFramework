/**
  ******************************************************************************
  * @file    usbh_adk_core.c
  * @author  Yuuichi Akagawa
  * @version V1.0.0
  * @date    2012/03/05
  * @brief   Android Open Accessory implementation
  ******************************************************************************
  * @attention
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  * <h2><center>&copy; COPYRIGHT (C)2012 Yuuichi Akagawa</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_adk_core.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_CATEGORY USB_ADK
#include "debug.h"

__ALIGN_BEGIN ADK_Machine_TypeDef ADK_Machine __ALIGN_END;
__ALIGN_BEGIN USB_Setup_TypeDef   ADK_Setup __ALIGN_END;

static USBH_Status USBH_ADK_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost);
static void USBH_ADK_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost);
static USBH_Status USBH_ADK_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost);
static USBH_Status USBH_ADK_ClassRequest(USB_OTG_CORE_HANDLE *pdev, void *phost);
static USBH_Status USBH_ADK_getProtocol(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);
static USBH_Status USBH_ADK_sendString(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t index, uint8_t* buff);
static USBH_Status USBH_ADK_switch(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);
static USBH_Status USBH_ADK_configAndroid(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

USBH_Class_cb_TypeDef USBH_ADK_cb =
{
	USBH_ADK_InterfaceInit,
	USBH_ADK_InterfaceDeInit,
	USBH_ADK_ClassRequest,
	USBH_ADK_Handle,
};

/**
  * @brief  USBH_ADK_Init
  *         Initialization for ADK class.
  * @param  manufacture: manufacturer name string(max 63 chars)
  * @param  model: model name string (max 63 chars)
  * @param  description: description string (max 63 chars)
  * @param  version: version string (max 63 chars)
  * @param  uri: URI string (max 63 chars)
  * @param  serial: serial number string (max 63 chars)
  * @retval None
  */
void USBH_ADK_Init(const char* manufacture, const char* model, const char* description, const char* version, const char* uri, const char* serial)
{
	strncpy(ADK_Machine.acc_manufacturer, manufacture, 64);
	ADK_Machine.acc_manufacturer[63] = '\0';
	strncpy(ADK_Machine.acc_model, model, 64);
	ADK_Machine.acc_model[63] = '\0';
	strncpy(ADK_Machine.acc_description, description, 64);
	ADK_Machine.acc_description[63] = '\0';
	strncpy(ADK_Machine.acc_version, version, 64);
	ADK_Machine.acc_version[63] = '\0';
	strncpy(ADK_Machine.acc_uri, uri, 64);
	ADK_Machine.acc_uri[63] = '\0';
	strncpy(ADK_Machine.acc_serial, serial, 64);
	ADK_Machine.acc_serial[63] = '\0';
	
	ADK_Machine.initstate = ADK_INIT_SETUP;
	ADK_Machine.state     = ADK_INITIALIZING; //ADK_ERROR;
	LOG("USB_ADK_Init");
}

/**
  * @brief  USBH_ADK_InterfaceInit
  *         Interface initialization for ADK class.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */
static USBH_Status USBH_ADK_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
	USBH_Status status = USBH_OK;
	
	ADK_Machine.inSize = 0;
	ADK_Machine.outSize = 0;
	
	LOG("USBH_ADK_InterfaceInit");
	return status ;
}

/**
  * @brief  USBH_ADK_InterfaceDeInit
  *         De-Initialize interface by freeing host channels allocated to interface
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval None
  */
void USBH_ADK_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
	LOG("USB_ADK_DeInit");
	ADK_Machine.initstate = ADK_INIT_SETUP;
	
	/* Switch to accessory mode,  Re-enumeration */
	//	if(ADK_Machine.state == ADK_INITIALIZING)
	//	{
	//		pdev->host.ConnSts = 1;
	//	}
	
	/* close bulk transfer pipe */
	if (ADK_Machine.hc_num_out)
	{
		USB_OTG_HC_Halt(pdev, ADK_Machine.hc_num_out);
		USBH_Free_Channel(pdev, ADK_Machine.hc_num_out);
		ADK_Machine.hc_num_out = 0;     /* Reset the Channel as Free */
	}
	if (ADK_Machine.hc_num_in)
	{
		USB_OTG_HC_Halt(pdev, ADK_Machine.hc_num_in);
		USBH_Free_Channel(pdev, ADK_Machine.hc_num_in);
		ADK_Machine.hc_num_in = 0;     /* Reset the Channel as Free */
	}
	
	//restore NAK retry limit to default value
	//	pdev->host.NakRetryLimit = USB_NAK_RETRY_ATTEMPTS;
}

/**
  * @brief  USBH_ADK_ClassRequest
  *         This function will only initialize the ADK state machine
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */
static USBH_Status USBH_ADK_ClassRequest(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
	USBH_HOST *pphost = phost;
	USBH_Status status = USBH_BUSY;
	
	switch (ADK_Machine.initstate)
	{
	case ADK_INIT_SETUP:
		LOG("USB_ADK_ClassRequest");
		// minimize NAK retry limit
		//		  	pdev->host.NakRetryLimit = USBH_ADK_NAK_RETRY_LIMIT;
		
		//check vaild device
		if (pphost->device_prop.Dev_Desc.idVendor == USB_ACCESSORY_VENDOR_ID &&
		    (pphost->device_prop.Dev_Desc.idProduct == USB_ACCESSORY_PRODUCT_ID ||
		     pphost->device_prop.Dev_Desc.idProduct == USB_ACCESSORY_ADB_PRODUCT_ID))
		{
			ADK_Machine.initstate = ADK_INIT_CONFIGURE_ANDROID;
		}
		else
		{
			ADK_Machine.initstate = ADK_INIT_GET_PROTOCOL;
			ADK_Machine.protocol = -1;
		}
		break;
		
	case ADK_INIT_GET_PROTOCOL:
		if (USBH_ADK_getProtocol(pdev, phost) == USBH_OK)
		{
			if (ADK_Machine.protocol >= 1)
			{
				ADK_Machine.initstate = ADK_INIT_SEND_MANUFACTURER;
				LOG("ADK:device supports protcol 1");
			}
			else
			{
				ADK_Machine.initstate = ADK_INIT_FAILED;
				LOG("ADK:could not read device protocol version");
			}
		}
		break;
	case ADK_INIT_SEND_MANUFACTURER:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_MANUFACTURER, (uint8_t*)ADK_Machine.acc_manufacturer) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SEND_MODEL;
			LOG("ADK:SEND_MANUFACTURER");
		}
		break;
	case ADK_INIT_SEND_MODEL:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_MODEL, (uint8_t*)ADK_Machine.acc_model) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SEND_DESCRIPTION;
			LOG("ADK:SEND_MODEL");
		}
		break;
	case ADK_INIT_SEND_DESCRIPTION:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_DESCRIPTION, (uint8_t*)ADK_Machine.acc_description) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SEND_VERSION;
			LOG("ADK:SEND_DESCRIPTION");
		}
		break;
	case ADK_INIT_SEND_VERSION:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_VERSION, (uint8_t*)ADK_Machine.acc_version) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SEND_URI;
			LOG("ADK:SEND_VERSION");
		}
		break;
	case ADK_INIT_SEND_URI:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_URI, (uint8_t*)ADK_Machine.acc_uri) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SEND_SERIAL;
			LOG("ADK:SEND_URI");
		}
		break;
	case ADK_INIT_SEND_SERIAL:
		if (USBH_ADK_sendString(pdev, phost, ACCESSORY_STRING_SERIAL, (uint8_t*)ADK_Machine.acc_serial) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_SWITCHING;
			LOG("ADK:SEND_SERIAL");
		}
		break;
	case ADK_INIT_SWITCHING:
		if (USBH_ADK_switch(pdev, phost) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_GET_DEVDESC;
			LOG("ADK:switch to accessory mode");
		}
		break;
		
	case ADK_INIT_GET_DEVDESC:
		if (USBH_Get_DevDesc(pdev , phost, USB_DEVICE_DESC_SIZE) == USBH_OK)
		{
			ADK_Machine.initstate = ADK_INIT_DONE;
			ADK_Machine.pid = pphost->device_prop.Dev_Desc.idProduct;
			//check vaild device
			if (pphost->device_prop.Dev_Desc.idVendor == USB_ACCESSORY_VENDOR_ID &&
			    (pphost->device_prop.Dev_Desc.idProduct == USB_ACCESSORY_PRODUCT_ID ||
			     pphost->device_prop.Dev_Desc.idProduct == USB_ACCESSORY_ADB_PRODUCT_ID))
			{
				ADK_Machine.initstate = ADK_INIT_CONFIGURE_ANDROID;
			}
			else
			{
				ADK_Machine.initstate = ADK_INIT_FAILED;
			}
		}
		break;
		
	case ADK_INIT_CONFIGURE_ANDROID:
		USBH_ADK_configAndroid(pdev, phost);
		ADK_Machine.initstate = ADK_INIT_DONE;
		break;
		
	case ADK_INIT_DONE:
		status = USBH_OK;
		ADK_Machine.state = ADK_IDLE;
		LOG("ADK:configuration complete");
		break;
		
	case ADK_INIT_FAILED:
		status = USBH_UNRECOVERED_ERROR;
		break;
		
	default:
		break;
	}
	
	return status;
}

/**
  * @brief  USBH_ADK_Handle
  *         ADK state machine handler
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */
static USBH_Status USBH_ADK_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
	USBH_Status status = USBH_BUSY;
	URB_STATE URB_Status;
	HC_STATUS HCD_Status;
	uint32_t HCD_GXferCnt;
	
	switch (ADK_Machine.state)
	{
	case ADK_IDLE:
		ADK_Machine.state = ADK_GET_DATA;
		
	// case ADK_SEND_DATA:
		// URB_Status = HCD_GetURB_State(pdev, ADK_Machine.hc_num_out);
		// HCD_Status = HCD_GetHCState(pdev, ADK_Machine.hc_num_out);
		// HCD_GXferCnt = HCD_GetXferCnt(pdev, ADK_Machine.hc_num_out);
		
		// if (ADK_Machine.outSize > 0)
		// {
		// USBH_BulkSendData(pdev, ADK_Machine.outbuff, ADK_Machine.outSize, ADK_Machine.hc_num_out);
		// ADK_Machine.outSize = 0;
		// }
		// ADK_Machine.state = ADK_GET_DATA;
		// break;
		
	case ADK_GET_DATA:
		URB_Status = HCD_GetURB_State(pdev, ADK_Machine.hc_num_in);
		HCD_Status = HCD_GetHCState(pdev, ADK_Machine.hc_num_in);
		HCD_GXferCnt = HCD_GetXferCnt(pdev, ADK_Machine.hc_num_in);
		// removed as URB in ERROR state can be simply recovered
		// if (URB_Status > URB_DONE)
			// break;
		USBH_BulkReceiveData(pdev, ADK_Machine.inbuff, USBH_ADK_DATA_SIZE, ADK_Machine.hc_num_in);
		ADK_Machine.state = ADK_IDLE;
		break;
		
	case ADK_BUSY:
		ADK_Machine.state = ADK_IDLE;
		ADK_Machine.outSize = 0;
		break;
		
	default:
		break;
	}
	status = USBH_OK;
	return status;
}

/**
  * @brief  USBH_ADK_getProtocol
  *         Inquiry protocol version number from Android device.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */
static USBH_Status USBH_ADK_getProtocol(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
	phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_VENDOR | USB_REQ_RECIPIENT_DEVICE;
	phost->Control.setup.b.bRequest = ACCESSORY_GET_PROTOCOL;
	phost->Control.setup.b.wValue.w = 0;
	phost->Control.setup.b.wIndex.w = 0;
	phost->Control.setup.b.wLength.w = 2;
	
	/* Control Request */
	return USBH_CtlReq(pdev, phost, (uint8_t*)&ADK_Machine.protocol, 2);
}

/**
  * @brief  USBH_ADK_sendString
  *         Send identifying string information to the Android device.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @param  index: String ID
  * @param  buff: Identifying string
  * @retval USBH_Status
  */
static USBH_Status USBH_ADK_sendString(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t index, uint8_t* buff)
{
	uint16_t length;
	length = (uint16_t)strlen(buff) + 1;
	
	phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_VENDOR | USB_REQ_RECIPIENT_DEVICE;
	phost->Control.setup.b.bRequest = ACCESSORY_SEND_STRING;
	phost->Control.setup.b.wValue.w = 0;
	phost->Control.setup.b.wIndex.w = index;
	phost->Control.setup.b.wLength.w = length;
	
	/* Control Request */
	return USBH_CtlReq(pdev, phost, buff, length);
}

/**
  * @brief  USBH_ADK_switch
  *         Request the Android device start up in accessory mode.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */
static USBH_Status USBH_ADK_switch(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
	phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_VENDOR | USB_REQ_RECIPIENT_DEVICE;
	phost->Control.setup.b.bRequest = ACCESSORY_START;
	phost->Control.setup.b.wValue.w = 0;
	phost->Control.setup.b.wIndex.w = 0;
	phost->Control.setup.b.wLength.w = 0;
	
	/* Control Request */
	return USBH_CtlReq(pdev, phost, 0, 0);
}

/**
  * @brief  USBH_ADK_configAndroid
  *         Setup bulk transfer endpoint and open channel.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */
static USBH_Status USBH_ADK_configAndroid(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
	USBH_HOST *pphost = phost;
	if (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80)
	{
		ADK_Machine.BulkInEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
		ADK_Machine.BulkInEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
	}
	else
	{
		ADK_Machine.BulkOutEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
		ADK_Machine.BulkOutEpSize  = pphost->device_prop.Ep_Desc[0] [0].wMaxPacketSize;
	}
	
	if (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress & 0x80)
	{
		ADK_Machine.BulkInEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
		ADK_Machine.BulkInEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
	}
	else
	{
		ADK_Machine.BulkOutEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
		ADK_Machine.BulkOutEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
	}
	LOG("ADK:configure bulk endpoint IN: 0x%02x OUT: 0x%02x", ADK_Machine.BulkInEp, ADK_Machine.BulkOutEp);
	
	ADK_Machine.hc_num_out = USBH_Alloc_Channel(pdev, ADK_Machine.BulkOutEp);
	ADK_Machine.hc_num_in  = USBH_Alloc_Channel(pdev, ADK_Machine.BulkInEp);
	
	/* Open the new channels */
	USBH_Open_Channel(pdev,
	                  ADK_Machine.hc_num_out,
	                  pphost->device_prop.address,
	                  pphost->device_prop.speed,
	                  EP_TYPE_BULK,
	                  ADK_Machine.BulkOutEpSize);
	                  
	USBH_Open_Channel(pdev,
	                  ADK_Machine.hc_num_in,
	                  pphost->device_prop.address,
	                  pphost->device_prop.speed,
	                  EP_TYPE_BULK,
	                  ADK_Machine.BulkInEpSize);
	                  
	return USBH_OK;
}

/**
  * @brief  USBH_ADK_write
  *         Send data to Android device.
  * @param  pdev: Selected device
  * @param  buff: send data
  * @param  len : send data length
  * @retval USBH_Status
  */
USBH_Status USBH_ADK_write(USB_OTG_CORE_HANDLE *pdev, const void* buff, uint16_t len)
{
	memcpy(ADK_Machine.outbuff, buff, len);
	ADK_Machine.outSize = len;
	
	USBH_BulkSendData(pdev, ADK_Machine.outbuff, ADK_Machine.outSize, ADK_Machine.hc_num_out);
	ADK_Machine.outSize = 0;
	
	return USBH_OK;
}

/**
  * @brief  USBH_ADK_read
  *         Receive data from  Android device.
  * @param  pdev: Selected device
  * @param  buff: receive data
  * @param  len : receive data buffer length
  * @retval received data length
  */
uint16_t USBH_ADK_read(USB_OTG_CORE_HANDLE *pdev, void* buff, uint16_t len)
{
	uint32_t xfercount;
	xfercount = HCD_GetXferCnt(pdev, ADK_Machine.hc_num_in);
	if (xfercount > 0)
	{
		if (len > xfercount)
			len = xfercount;
		memcpy(buff, ADK_Machine.inbuff, len);
		ADK_Machine.inSize = 0;
		pdev->host.XferCnt[ADK_Machine.hc_num_in] -= xfercount; // we shouldn't read less than whole packet from FIFO
		return len;
	}
	else
	{
		return 0;
	}
}

/**
  * @brief  USBH_ADK_getStatus
  *         Return ADK_Machine.state
  * @param  None
  * @retval ADK_Machine.state
  */
ADK_State USBH_ADK_getStatus(void)
{
	return ADK_Machine.state;
}
