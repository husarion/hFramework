/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#ifdef __cplusplus
#define NO_USING_HFRAMEWORK
#include <hFramework.h>
#endif
#include <string.h>
#include <stdbool.h>

#ifndef arduino_cost
#define arduino_cost

// bases
//const int HEX = 16;
//const int DEC = 10;
//const int OCT = 8;
//const int BIN = 2;

// digital states
//const int LOW = 0;
//const int HIGH = 1;

// pin modes
const int INPUT = 1;
const int OUTPUT = 2;
const int INPUT_PULLUP = 3;

const int A0 = 40;
const int A1 = 41;
const int A2 = 42;
const int A3 = 43;
const int A4 = 44;
const int A5 = 45;
const int A6 = 46;
const int A7 = 47;
const int A8 = 48;
const int A9 = 49;
const int A10 = 50;

//typedef unsigned int word;
//typedef bool boolean;


//namespace hFramework {

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
#define min(a, b) (a<b?a:b)
#define max(a, b) (a>b?a:b)

#ifdef __cplusplus
extern "C" {
#endif
void pinMode(int pinIndex, int value);
int analogRead(int pinIndex);
int digitalRead(int pinIndex);
void digitalWrite(int pinIndex, int value);

unsigned long millis(void);
unsigned long micros(void);
void delay(int ms);
void delayMicroseconds(unsigned int us);

#ifdef __cplusplus
}


namespace hFramework {
struct ArduinoSerial {
    hSerial& hserial;

    ArduinoSerial(hSerial& s): hserial(s) {}

    int available() {
        return hserial.available();
    }

    void begin(int baudrate) {
        hserial.init(baudrate);
    }

    void print(int num, int base=10) {
        switch (base) {
        case 2:
            hserial.printf("%b", num);
            break;
        //case 8:
        //    hserial.printf("%o", num);
        //    break;
        case 10:
            hserial.printf("%d", num);
            break;
        case 16:
            hserial.printf("%x", num);
            break;
        }
    }

    void print(double num) {
        hserial.printf("%f", num);
    }

    void print(char c) {
        hserial.printf("%c", c);
    }

    void print(const char* s) {
        hserial.printf("%s", s);
    }
	
	void println(const char* s) {
        hserial.printf("%s\n", s);
    }
	
	void println() {
        hserial.printf("\n");
    }
	
	long random(long max);
	long random(long min, long max);

    operator bool() {
        return true;
    }
};

}

extern hFramework::ArduinoSerial Serial;

#endif

#endif