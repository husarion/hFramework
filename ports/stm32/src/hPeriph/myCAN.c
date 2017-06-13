/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "myCAN.h"
#include "stm32f4xx.h"
#include "system.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

xQueueHandle canRxQueue;

CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
CanTxMsg TxMessage;

void CAN1_RX0_IRQHandler(void)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	int i;
	CanRxMsg RxMessage;

	RxMessage.StdId = 0;
	RxMessage.ExtId = 0;
	RxMessage.IDE = 0;
	RxMessage.DLC = 0;
	RxMessage.FMI = 0;
	for (i = 0; i < 8; i++)
	{
		RxMessage.Data[i] = 0;
	}

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	if (pdTRUE == xQueueSendToBackFromISR(canRxQueue, (CAN_frameRx*)(&RxMessage), &xHigherPriorityTaskWoken))
	{
		if (xHigherPriorityTaskWoken == pdTRUE)
		{
			taskYIELD();
		}
	}
}

void myCAN_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if (0 == canRxQueue)
	{
		canRxQueue = xQueueCreate(10, sizeof(CanRxMsg));
	}
	else
	{
		xQueueReset(canRxQueue);
	}

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	CAN_DeInit(CAN1);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//moze byc tez loopback

	//	/* CAN Baudrate = 175kbps (CAN clocked at 42MHz) */
	//	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	//	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	//	CAN_InitStructure.CAN_Prescaler = 16;

	/* CAN Baudrate = 1Mbps (CAN clocked at 42MHz) */
	//	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	//	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	//	CAN_InitStructure.CAN_Prescaler = 3;

	/* CAN Baudrate = 500kbps (CAN clocked at 42MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 6;

	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void myCAN_sendFrame(CAN_frameTx* frame)
{
	uint8_t transmit_mailbox = 4;

	//wait for empty mailbox
	while (4 == transmit_mailbox)
	{
		/* Select one empty transmit mailbox */
		if ((CAN1->TSR & CAN_TSR_TME0) == CAN_TSR_TME0)
		{
			transmit_mailbox = 0;
		}
		else if ((CAN1->TSR & CAN_TSR_TME1) == CAN_TSR_TME1)
		{
			transmit_mailbox = 1;
		}
		else if ((CAN1->TSR & CAN_TSR_TME2) == CAN_TSR_TME2)
		{
			transmit_mailbox = 2;
		}
		else
		{
			transmit_mailbox = 4;
		}
	}

	CAN_Transmit(CAN1, (CanTxMsg*)frame);

	//	frame2.StdId = 0x11;
	//	frame2.RTR = CAN_RTR_DATA;
	//	frame2.IDE = CAN_ID_STD;
	//	frame2.DLC = 2;
	//	frame2.Data[0] = 0xCA;
	//	frame2.Data[1] = 0xFE;
	//
	//	CAN_Transmit(CAN1, &frame2);
}

uint8_t myCAN_waitFrame(CAN_frameRx* frame, uint32_t timeout)
{
	return xQueueReceive(canRxQueue, frame, msToTicks(timeout)) == pdTRUE;
}
