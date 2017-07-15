/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"

#include "myADC.h"

#include "system.h"

namespace hFramework
{

#if BOARD(ROBOCORE)
hUSB Usb;

hSensor_i2c     hBaseSens1(hSens_ID_1);
hSensor_i2c     hBaseSens2(hSens_ID_2);
hSensor_serial  hBaseSens3(hSens_ID_3);
hSensor_simple  hBaseSens4(hSens_ID_4);
hSensor_simple  hBaseSens5(hSens_ID_5);

hLegoSensor_i2c    hSens1(hBaseSens1);
hLegoSensor_i2c    hSens2(hBaseSens2);
hLegoSensor_serial hSens3(hBaseSens3);
hLegoSensor_simple hSens4(hBaseSens4);
hLegoSensor_simple hSens5(hBaseSens5);

hMotor          hMotA(hMotor_ID_1);
hMotor          hMotB(hMotor_ID_2);
hMotor          hMotC(hMotor_ID_3);
hMotor          hMotD(hMotor_ID_4);
hMotor          hMotE(hMotor_ID_5);
hMotor          hMotF(hMotor_ID_6);

hMotor *port_motors[] = { &hMot1, &hMot2, &hMot3, &hMot4, &hMot5, &hMot6 };

hButton_int hBtn1(hButton_ID_SW1);
hButton_int hBtn2(hButton_ID_SW2);

hSerial Edison(hSerial_ID_EDISON);

hI2C hExtI2C(hI2C_ID_EXT);
hSPI hExtSPI(hSPI_ID_EXT);
hExtClass hExt1(hExt_ID_1);
hExtClass hExt2(hExt_ID_2);

hCAN CAN;
#elif BOARD(CORE2)
hSensor_i2c     hSens1(hSens_ID_1);
hSensor_i2c     hSens2(hSens_ID_2);
hSensor_serial  hSens3(hSens_ID_3);
hSensor_serial  hSens4(hSens_ID_4);
hSensor_simple  hSens5(hSens_ID_5);
hSensor_simple  hSens6(hSens_ID_6);

hMotor          hMotA(hMotor_ID_1);
hMotor          hMotB(hMotor_ID_2);
hMotor          hMotC(hMotor_ID_3);
hMotor          hMotD(hMotor_ID_4);

hMotor *port_motors[] = { &hMot1, &hMot2, &hMot3, &hMot4 };

hButton_int hBtn1(hButton_ID_SW1);
hButton_int hBtn2(hButton_ID_SW2);
hButton     hCfg(hButton_ID_SW3);

hSerial RPi(hSerial_ID_RPI);

hExtClass hExt;

hCAN CAN;

hServoModuleClass hServo;
#elif BOARD(CORE2MINI)
hSensor_i2c     hSens1(hSens_ID_1);
hSensor_serial  hSens2(hSens_ID_2);
hSensor_serial  hSens3(hSens_ID_3);

hMotor          hMotA(hMotor_ID_1);
hMotor          hMotB(hMotor_ID_2);

hMotor *port_motors[] = { &hMot1, &hMot2 };

hButton_int hBtn1(hButton_ID_SW1);
hButton     hCfg(hButton_ID_SW3);

hSerial RPi(hSerial_ID_RPI);

hServoModuleClass hServo;
#else
#  error no board
#endif

hSD SD;

hLEDClass hLED1(0);
hLEDClass hLED2(1);
hLEDClass hLED3(2);

hSerial Serial(hSerial_ID_FT);

void portInit()
{
	myADC_init();

	// Edison.init(460800, NONE, ONE, FLOW_CONTROL_RTS_CTS);
	Serial.init(460800);
#if BOARD(ROBOCORE)
	Edison.init(460800);
#elif BOARD(CORE2)
	RPi.init(500000);
#elif BOARD(CORE2MINI)
	RPi.init(500000);
#else
#  error no board
#endif

	sys.setLogDev(&Serial);
}

void hMainInit();
}

void initFunc(void*)
{
	hFramework::portInit();
	hFramework::hMainInit();
#if BOARD(ROBOCORE)
	sys.taskCreate(sys_idle_task, 1, 500, "hIdle");
#endif
	vTaskDelete(NULL);
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	if (xTaskCreate(initFunc, (const char*)"start", 400, 0, configMAX_PRIORITIES - 1, 0) == pdTRUE)
	{
	}
	vTaskStartScheduler();
	return 0;
}

extern "C" void HardFault_Handler()
{
	sys.fail_log("HardFault_Handler\r\n");
	sys.fault_handler();
}
extern "C" void NMI_Handler()
{
	sys.fail_log("NMI_Handler\r\n");
	sys.fault_handler();
}
extern "C" void MemManage_Handler()
{
	sys.fail_log("MemManage_Handler\r\n");
	sys.fault_handler();
}
extern "C" void BusFault_Handler()
{
	sys.fail_log("BusFault_Handler\r\n");
	sys.fault_handler();
}
extern "C" void UsageFault_Handler()
{
	sys.fail_log("UsageFault_Handler\r\n");
	sys.fault_handler();
}
