/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <hTypes.h>


#include <hUSB.h>
#include <hSerial.h>
#include <hSensor.h>
#include <hLegoSensor.h>
#include <hButton.h>
#include <hCAN.h>
#include <hExt.h>
#include <hLED.h>
#include <hMotor.h>
#include <hSD.h>
#include <hServoModule.h>

namespace hFramework
{

extern hLEDClass hLED1;
extern hLEDClass hLED2;
extern hLEDClass hLED3;

static hLEDClass& LED1 __attribute__((deprecated)) = hLED1;
static hLEDClass& LED2 __attribute__((deprecated)) = hLED2;
static hLEDClass& LED3 __attribute__((deprecated)) = hLED3;

extern hSD SD;

//// ROBOCORE
#if BOARD(ROBOCORE)
extern hSensor_i2c    hBaseSens1;
extern hSensor_i2c    hBaseSens2;
extern hSensor_serial hBaseSens3;
extern hSensor_simple hBaseSens4;
extern hSensor_simple hBaseSens5;

extern hLegoSensor_i2c    hSens1;
extern hLegoSensor_i2c    hSens2;
extern hLegoSensor_serial hSens3;
extern hLegoSensor_simple hSens4;
extern hLegoSensor_simple hSens5;

extern hMotor hMotA;
extern hMotor hMotB;
extern hMotor hMotC;
extern hMotor hMotD;
extern hMotor hMotE;
extern hMotor hMotF;

static hMotor& hMot1 = hMotA;
static hMotor& hMot2 = hMotB;
static hMotor& hMot3 = hMotC;
static hMotor& hMot4 = hMotD;
static hMotor& hMot5 = hMotE;
static hMotor& hMot6 = hMotF;

extern hExtClass hExt1;
extern hExtClass hExt2;

extern hButton_int hBtn1; //!< SW1 button
extern hButton_int hBtn2; //!< SW2 button

extern hSerial Edison; //!< Serial link to <a href="https://wiki.robocore.io/edison:start">Edison</a>. With normal Edison firmware, it is internal to the hCloudClient.

extern hCAN CAN;

extern hUSB Usb;

//// CORE2
#elif BOARD(CORE2)
extern hSensor_i2c    hSens1;
extern hSensor_i2c    hSens2;
extern hSensor_serial hSens3;
extern hSensor_serial hSens4;
extern hSensor_simple hSens5;
extern hSensor_simple hSens6;

extern hMotor hMotA;
extern hMotor hMotB;
extern hMotor hMotC;
extern hMotor hMotD;

static hMotor& hMot1 = hMotA;
static hMotor& hMot2 = hMotB;
static hMotor& hMot3 = hMotC;
static hMotor& hMot4 = hMotD;

extern hExtClass hExt;

extern hButton_int hBtn1; //!< SW1 button
extern hButton_int hBtn2; //!< SW2 button
extern hButton     hCfg; //!< SW3 button

extern hSerial RPi;

extern hServoModuleClass hServo;
static __attribute__((deprecated)) hServoModuleClass& hServoModule = hServo;

extern hCAN CAN;

//// CORE2 mini
#elif BOARD(CORE2MINI)
extern hSensor_i2c    hSens1;
extern hSensor_serial hSens2;
extern hSensor_serial hSens3;

extern hMotor hMotA;
extern hMotor hMotB;

static hMotor &hMot1 = hMotA;
static hMotor &hMot2 = hMotB;

extern hButton_int hBtn1; //!< SW1 button
extern hButton     hCfg; //!< SW3 button

extern hSerial RPi;

extern hServoModuleClass hServo;
static __attribute__((deprecated)) hServoModuleClass& hServoModule = hServo;

#else
#  error no board
#endif

extern hSerial Serial; //!< RoboCORE <a href="https://wiki.robocore.io/hardware:usb_device">USB-serial</a>.

}

#endif
