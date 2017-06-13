/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __YCM_H__
#define __YCM_H__

#ifdef YCM

#include "../ports/stm32/src/hSPI.cpp"
#include "../ports/stm32/src/hMutex.cpp"
#include "../ports/stm32/src/init.cpp"
#include "../ports/stm32/src/hExt.cpp"
#include "../ports/stm32/src/hUSB/hUSB.cpp"
#include "../ports/stm32/src/hUSB/usb/usbh_adk_core.c"
#include "../ports/stm32/src/hUSB/usb/usb_bsp.c"
#include "../ports/stm32/src/hUSB/usb/usbh_usr.c"
#include "../ports/stm32/src/hSerial.cpp"
#include "../ports/stm32/src/runTimeStatsInit.c"
#include "../ports/stm32/src/hMotorPimpl.cpp"
#include "../ports/stm32/src/hGPIO.cpp"
#include "../ports/stm32/src/hI2C.cpp"
#include "../ports/stm32/src/hSystem.cpp"
#include "../ports/stm32/src/hLED.cpp"
#include "../ports/stm32/src/hStorage.cpp"
#include "../ports/stm32/src/syscalls.c"
#include "../ports/stm32/src/hSensor.cpp"
#include "../ports/stm32/src/hSD.cpp"
#include "../ports/stm32/src/hButton.cpp"
#include "../ports/stm32/src/hMotor.cpp"
#include "../ports/stm32/src/hPeriph/myDAC.c"
#include "../ports/stm32/src/hPeriph/myTimer.c"
#include "../ports/stm32/src/hPeriph/myPWM.c"
#include "../ports/stm32/src/hPeriph/myGPIO.c"
#include "../ports/stm32/src/hPeriph/myUSART.c"
#include "../ports/stm32/src/hPeriph/mySPI.c"
#include "../ports/stm32/src/hPeriph/myUSART_poll.c"
#include "../ports/stm32/src/hPeriph/myI2C.c"
#include "../ports/stm32/src/hPeriph/myDMA.c"
#include "../ports/stm32/src/hPeriph/myUSART_isr.c"
#include "../ports/stm32/src/hPeriph/myCAN.c"
#include "../ports/stm32/src/hPeriph/myUSART_dma.c"
#include "../ports/stm32/src/hPeriph/myEncoder.c"
#include "../ports/stm32/src/hPeriph/myADC.c"
#include "../ports/stm32/src/hPeriph/myEXTI.c"
#include "../ports/stm32/src/hCAN.cpp"
#include "../ports/stm32/src/hServoModule.cpp"
#include "../src/IServo.cpp"
#include "../src/Other/myprintf.cpp"
#include "../src/Other/hCyclicBuffer.cpp"
#include "../src/Other/hPIDRegulator.cpp"
#include "../src/Other/hScriptingLanguage.cpp"
#include "../src/Other/hUtils.cpp"
#include "../src/Other/hPrintfDev.cpp"
#include "../src/Other/hPrintf.cpp"
#include "../src/Other/hPID.cpp"
#include "../src/Other/hStreamDev.cpp"
#include "../src/Other/hMath.cpp"
#include "../src/Other/hSoftwareI2C.cpp"
#include "../src/II2C.cpp"
#include "../src/hSystem/gqueue.cpp"
#include "../src/hSystem/main.cpp"
#include "../src/hSystem/system.cpp"
#include "../src/hSystem/hEventBits.cpp"

#endif
#endif

