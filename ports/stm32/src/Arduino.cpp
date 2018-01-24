#include <hFramework.h>
#include <stdint.h>
#include "Arduino.h"

//hFramework::ArduinoSerial Serial(Serial);

#if BOARD(CORE2)
static auto pins = std::make_tuple(hExt.serial.pinRx, hExt.serial.pinTx,
                                   hExt.spi.pinSck, hExt.spi.pinMiso, hExt.spi.pinMosi,
                                   hExt.i2c.pinSda, hExt.i2c.pinScl,
                                   0, 0, 0, 0, 0, 0,
                                   hSens1.pin2, hSens1.pin3, hSens1.pin4, hSens2.pin2,
                                   hSens2.pin3, hSens2.pin4, hSens3.pin2, hSens3.pin3,
                                   hSens3.pin4, hSens4.pin2, hSens4.pin3, hSens4.pin4,
                                   hSens5.pin2, hSens5.pin3, hSens5.pin4, hSens6.pin2,
                                   hSens6.pin3, hSens6.pin4,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   hExt.pin1, hExt.pin2, hExt.pin3, hExt.pin4, hExt.pin5,
                                   hSens1.pin1, hSens2.pin1, hSens3.pin1,
                                   hSens4.pin1, hSens5.pin1, hSens6.pin1);
#define H_DIGITAL_PINS 0, 1, 2, 3, 4, 5, 6, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
#define H_ANALOG_PINS A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10
#else
#define H_DIGITAL_PINS
#define H_ANALOG_PINS
static auto pins = std::make_tuple();
#endif

template <typename T>
static __attribute__((noreturn)) T& getPin(int index, const char* error)
{
	sys.fail_log("ERROR: pin %d doesn't support %s", index, error);
	sys.fault_handler();
	abort();
}

template <typename T, int pin, int... allowed>
static T& getPin(int index, const char* error)   // this should result in unrolled loop
{
	if (index == pin)
	{
		return std::get<pin>(pins);
	}
	else
	{
		return getPin<T, allowed...>(index, error);
	}
}

extern "C" {
	unsigned long millis(void)
	{
		return sys.getRefTime();
	}

	unsigned long micros(void)
	{
		return sys.getRefTime() * 1000;
	}

	void delay(int ms)
	{
		sys.delay(ms);
	}

	void delayMicroseconds(unsigned int us)
	{
		sys.delayUs(us);
	}

	void digitalWrite(int pinIndex, int value)
	{
		hGPIO& gpio = getPin<hGPIO, H_DIGITAL_PINS>(pinIndex, "digitalWrite");
		gpio.write(value);
	}

	int digitalRead(int pinIndex)
	{
		hGPIO& gpio = getPin<hGPIO, H_DIGITAL_PINS>(pinIndex, "digitalRead");
		return gpio.read();
	}

	int analogRead(int pinIndex)
	{
		hGPIO_adc& gpio = getPin<hGPIO_adc, H_ANALOG_PINS>(pinIndex, "analogRead");
		gpio.enableADC();
		return gpio.analogReadRaw() / 4; // 0..4095 -> 0..1023
	}

	/*void analogWrite(int pinIndex, int value)
	{
		value %= 255;
		hServoModule.enablePower();
#if BOARD(CORE2)
		switch (pinIndex)
		{
		case 7:
			hServoModule.servo1.setPeriod(1000);
			hServoModule.servo1.setWidth((value + 1) / 255 * 1000);
			break;
		case 8:
			hServoModule.servo2.setPeriod(1000);
			hServoModule.servo2.setWidth((value + 1) / 255 * 1000);
			break;
		case 9:
			hServoModule.servo3.setPeriod(1000);
			hServoModule.servo3.setWidth((value + 1) / 255 * 1000);
			break;
		case 10:
			hServoModule.servo4.setPeriod(1000);
			hServoModule.servo4.setWidth((value + 1) / 255 * 1000);
			break;
		case 11:
			hServoModule.servo5.setPeriod(1000);
			hServoModule.servo5.setWidth((value + 1) / 255 * 1000);
			break;
		case 12:
			hServoModule.servo6.setPeriod(1000);
			hServoModule.servo6.setWidth((value + 1) / 255 * 1000);
			break;
		default:
			sys.fail_log("ERROR: pin %d doesn't support PWM", pinIndex);
		};
#endif
	}*/

//int hFramework::pulseIn(int pinIndex, int value, unsigned int timeout) {
//	unsigned int time = sys.getRefTime();
//	unsigned int time_us;
//	pinMode(pinIndex, INPUT);
//	while(digitalRead(pinIndex) != value) {
//		if(time+timeout/1000 < sys.getRefTime()) {
//			return 0;
//		}
//	}
//	time_us = sys.getUsTimVal();
//	while(digitalRead(pinIndex) == value);
//	return sys.getUsTimVal() - time_us;
//}

	void pinMode(int pinIndex, int value)
	{
		hGPIO& gpio = getPin<hGPIO, H_DIGITAL_PINS>(pinIndex, "pinMode");
		switch (value)
		{
		case INPUT:
			gpio.setIn();
			break;
		case OUTPUT:
			gpio.setOut();
			break;
		case INPUT_PULLUP:
			gpio.setIn_pu();
			break;
		default:
			sys.fail_log("ERROR: invalid pin mode %d", value);
			sys.fault_handler();
		}
	}
}

hFramework::String::String(int n)
{
	char buf [50];
	sprintf(buf, "%d", n);
	this->assign(buf);
}

hFramework::String::String(unsigned int n)
{
	char buf [50];
	sprintf(buf, "%d", n);
	this->assign(buf);
}

hFramework::String::String(float n)
{
	char buf [50];
	sprintf(buf, "%f", n);
	this->assign(buf);
}

hFramework::String::String(double n)
{
	char buf [50];
	sprintf(buf, "%f", n);
	this->assign(buf);
}

hFramework::String::String(unsigned long n)
{
	char buf [50];
	sprintf(buf, "%lu", n);
	this->assign(buf);
}

char hFramework::String::charAt(size_t poz)
{
	return this->at(poz);
}

void hFramework::String::setCharAt(size_t index, char c)
{
	this->insert(index, c, sizeof(c));
}
