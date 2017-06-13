/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "hGPIO.h"
#include "myGPIO.h"
#include "myADC.h"
#include "myEXTI.h"
#include "hUtils.h"
#include "hSystem.h"
#include "ycm.h"
#include "debug.h"

namespace hFramework
{

//==================================hGPIO================================================
void hGPIO::init()
{
	myGPIO_config(nr);
}

void hGPIO_int::init()
{
	myEXTI_config(nr, SEMPHR_NORMAL);
	myGPIO_EXTIconfig(nr, EXTI_FALLING);
	myGPIO_EXTI_off(nr);

	myGPIO_config(nr);
}

// hGPIO
hGPIO::hGPIO(uint8_t nr) : nr(nr)
{
}

void hGPIO::setOut()
{
	myGPIO_dir_out(nr);
	myGPIO_outType_pp(nr);
	myGPIO_pupd_none(nr);
}
void hGPIO::setIn()
{
	myGPIO_dir_in(nr);
	myGPIO_pupd_none(nr);
}
void hGPIO::setIn_pu()
{
	myGPIO_dir_in(nr);
	myGPIO_pupd_pu(nr);
}
void hGPIO::setIn_pd()
{
	myGPIO_dir_in(nr);
	myGPIO_pupd_pd(nr);
}
void hGPIO::setOutOD()
{
	myGPIO_dir_out(nr);
	myGPIO_outType_od(nr);
	myGPIO_pupd_none(nr);
}
void hGPIO::setOutOD_pu()
{
	myGPIO_dir_out(nr);
	myGPIO_outType_od(nr);
	myGPIO_pupd_pu(nr);
}
void hGPIO::setOutOD_pd()
{
	myGPIO_dir_out(nr);
	myGPIO_outType_od(nr);
	myGPIO_pupd_pd(nr);
}
bool hGPIO::read()
{
	return myGPIO_read(nr);
}
void hGPIO::write(bool value)
{
	myGPIO_write(nr, value);
}
void hGPIO::toggle()
{
	myGPIO_toggle(nr);
}
void hGPIO::reset()
{
	init();
}

// hGPIO_int
hGPIO_int::hGPIO_int(uint8_t nr) : hGPIO(nr)
{
}

void hGPIO_int::interruptOff()
{
	myGPIO_EXTI_off(nr);
}

void hGPIO_int::_interruptOn(InterruptEdge edge, CountingMode mode, const std::function<void()>& handler)
{
	myGPIO_EXTI_off(nr);
	myEXTI_config(nr, mode == CountingMode::Normal ? SEMPHR_NORMAL : SEMPHR_COUNTING, handler);
	interruptSetEdge(edge);
	myGPIO_EXTI_on(nr);
	interruptWait(0);
}

void hGPIO_int::interruptSetEdge(InterruptEdge edge)
{
	switch (edge)
	{
	case InterruptEdge::Rising: myGPIO_EXTIconfig(nr, EXTI_RISING); break;
	case InterruptEdge::Falling: myGPIO_EXTIconfig(nr, EXTI_FALLING); break;
	case InterruptEdge::Both: myGPIO_EXTIconfig(nr, EXTI_RISING_FALLING); break;
	default: break;
	}
}

bool hGPIO_int::interruptWait(uint32_t timeout)
{
	return myEXTI_waitIntrpt(nr, timeout);
}

// hGPIO_adc
hGPIO_adc::hGPIO_adc(uint8_t nr) : hGPIO(nr)
{
}

void hGPIO_adc::enableADC()
{
	myADC_config(nr);
}
void hGPIO_adc::disableADC()
{
	myGPIO_config(nr);
	// myADC_off(nr);
}



float hGPIO_adc::analogReadVoltage()
{
#if BOARD(ROBOCORE)
	return ((float)(2 * analogReadRaw())) / 2675;
#elif BOARD(CORE2MINI) || BOARD(CORE2)
	return ((float)(2 * analogReadRaw())) / 2520; //this parameter = 8192/Vref
#else
#  error no board
#endif
}
uint16_t hGPIO_adc::analogReadRaw()
{
	return myADC_readPin(nr);
}

// hGPIO_super
hGPIO_super::hGPIO_super(uint8_t nr) : hGPIO(nr), hGPIO_int(nr), hGPIO_adc(nr)
{
}
void hGPIO_super::enableADC()
{
	myGPIO_EXTI_off(nr);
	myADC_config(nr);
}
void hGPIO_super::disableADC()
{
	myGPIO_EXTI_off(nr);
	myGPIO_config(nr);
	// myADC_off(nr);
}

}
