/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HUTILS_H__
#define __HUTILS_H__

#include <stdarg.h>
#include <stdlib.h>

#include "hTypes.h"

#ifndef HFRAMEWORK_COMPILE
#define FORCE_LINK(x) static void* __ ## x ## _force_link __attribute__((unused)) = (void*)&x
extern "C" {
	// to force proper linking with syscalls
	void _exit(int n);
	FORCE_LINK(_exit);
}
#endif

namespace hFramework
{

uint16_t crc16_update(uint16_t crc, uint8_t data);
uint16_t crc16_update_block(uint16_t crc, const void* data, int length);
uint16_t crc16_calc(const void* data, int length);

/**
 * @brief Return absolute value
 * @param x value
 */
template<class T>
T abs(T x)
{
	if (x >= 0)
		return x;
	return -x;
}

/**
 * @brief Return minimum from two values
 * @param a first value
 * @param b second value
 */
template<class T>
T min(T a, T b)
{
	if (a < b)
		return a;
	return b;
}

/**
 * @brief Return maximum from two values
 * @param a first value
 * @param b second value
 */
template<class T>
T max(T a, T b)
{
	if (a > b)
		return a;
	return b;
}

/**
 * @brief Limit value to a specific range. The value will be set to minimum
 * if is less then min parameter and to maximum if it is greater that max parameter.
 * @param val value to be limited
 * @param min minimum value
 * @param max maximum value
 */
template<class T>
T saturate(T val, T min, T max)
{
	if (val > max)
		val = max;
	else if (val < min)
		val = min;
	return val;
}

/**
 * @brief Map value from one range to another.
 * @param val value to be converted
 * @param fromMin minimum value of the base range
 * @param fromMax maximum value of the base range
 * @param mapMin minimum value of the destination range
 * @param mapMax maximum value of the destination range
 */
template<class T>
T map(T value, T fromMin, T fromMax, T mapMin, T mapMax)
{
	return (value - fromMin) * (mapMax - mapMin) / (fromMax - fromMin) + mapMin;
}

inline static uint16_t SWAP16(uint16_t x)
{
	return ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
}

inline static int16_t SWAP16(int16_t x)
{
	return ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
}

#ifndef  _M_LN2
#define  _M_LN2      0.693147180559945309417
#endif
#ifndef  M_E
#define  M_E         2.7182818284590452354
#endif
#ifndef  M_LOG2E
#define  M_LOG2E     1.4426950408889634074
#endif
#ifndef  M_LOG10E
#define  M_LOG10E    0.43429448190325182765
#endif
#ifndef  M_LN2
#define  M_LN2       _M_LN2
#endif
#ifndef  M_LN10
#define  M_LN10      2.30258509299404568402
#endif
#ifndef  M_PI
#define  M_PI        3.14159265358979323846
#endif
#ifndef  M_TWOPI
#define  M_TWOPI     (M_PI * 2.0)
#endif
#ifndef  M_PI_2
#define  M_PI_2      1.57079632679489661923
#endif
#ifndef  M_PI_4
#define  M_PI_4      0.78539816339744830962
#endif
#ifndef  M_3PI_4
#define  M_3PI_4     2.3561944901923448370E0
#endif
#ifndef  M_SQRTPI
#define  M_SQRTPI    1.77245385090551602792981
#endif
#ifndef  M_1_PI
#define  M_1_PI      0.31830988618379067154
#endif
#ifndef  M_2_PI
#define  M_2_PI      0.63661977236758134308
#endif
#ifndef  M_2_SQRTPI
#define  M_2_SQRTPI  1.12837916709551257390
#endif
#ifndef  M_SQRT2
#define  M_SQRT2     1.41421356237309504880
#endif
#ifndef  M_SQRT1_2
#define  M_SQRT1_2   0.70710678118654752440
#endif
#ifndef  M_LN2LO
#define  M_LN2LO     1.9082149292705877000E-10
#endif
#ifndef  M_LN2HI
#define  M_LN2HI     6.9314718036912381649E-1
#endif
#ifndef  M_SQRT3
#define  M_SQRT3     1.73205080756887719000
#endif
#ifndef  M_IVLN10
#define  M_IVLN10    0.43429448190325182765     /*  1     /  log(10)  */
#endif
#ifndef  M_LOG2_E
#define  M_LOG2_E    _M_LN2
#endif
#ifndef  M_INVLN2
#define  M_INVLN2    1.4426950408889633870E0    /*  1     /  log(2)   */
#endif

inline static float r2d(float rad)
{
	return rad * 180.0f / M_PI;
}
inline static float d2r(float deg)
{
	return deg * M_PI / 180.0f;
}

#ifdef YCM
#include "ycm.h"
#endif

}

#endif /* __HUTILS_H__ */
