/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef DEFINE_PORT____
#define DEFINE_PORT____(name,nr,port,pin,adc)
#endif
#ifndef DEFINE_PORT_IRQ
#define DEFINE_PORT_IRQ(name,nr,port,pin,adc,irq)
#endif
#ifndef DEFINE_EXTI
#define DEFINE_EXTI(name)
#endif
#ifndef DEFINE_ADC
#define DEFINE_ADC(adcNum, channel)
#endif

#include <hTypes.h>

#if BOARD_VERSION_EQ(CORE2,0,1,0)
#include "myGPIO_0_1_0_core2.h"
#elif BOARD_VERSION_EQ(CORE2MINI,0,1,0)
#include "myGPIO_0_1_0_core2mini.h"
#elif BOARD_VERSION_EQ(CORE2MINI,0,2,0)
#include "myGPIO_0_2_0_core2mini.h"
#elif BOARD_VERSION_EQ(CORE2MINI,1,0,0)
#include "myGPIO_0_2_0_core2mini.h"
#elif BOARD_VERSION_EQ(ROBOCORE,1,0,0)
#include "myGPIO_1_0_0_robocore.h"
#elif BOARD_VERSION_EQ(CORE2,0,2,0)
#include "myGPIO_0_2_0_core2.h"
#elif BOARD_VERSION_EQ(CORE2,1,0,0)
#include "myGPIO_1_0_0_core2.h"
#else
#error no board
#endif

#undef DEFINE_PORT____
#undef DEFINE_PORT_IRQ
#undef DEFINE_EXTI
#undef DEFINE_ADC
