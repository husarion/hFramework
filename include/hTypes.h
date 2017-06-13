/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HTYPES_H__
#define __HTYPES_H__

#include <stdint.h>
#include <stddef.h>

#include <defines.h>

// #ifdef __cplusplus
// template <typename T, size_t N>
// constexpr size_t arraySize(T(&)[N])
// {
	// return N;
// }
// #endif

#define COUNT_OF(x) (sizeof(x)/sizeof((x)[0]))

#ifdef __cplusplus

extern "C" {
#endif

#define VER(a,b,c) (((a) << 16) | ((b) << 8) | (c))

#define CURVER() VER(BOARD_VERSION_A,BOARD_VERSION_B,BOARD_VERSION_C)

/**
 * @brief Check if current board version is equal to passed.
 */
#define BOARD_VERSION_EQ(type,a,b,c) (BOARD(type) && (CURVER() == VER(a,b,c)))

/**
 * @brief Check if current board version is newer or equal to passed.
 */
#define BOARD_VERSION_MIN(type,a,b,c) (BOARD(type) && (CURVER() >= VER(a,b,c)))

typedef struct
{
	uint32_t StdId;
	uint32_t ExtId;
	uint8_t IDE;
	uint8_t RTR;
	uint8_t DLC;
	uint8_t data[8];
	uint8_t FMI;
} CAN_frameRx;

typedef struct
{
	uint32_t StdId; //Standard identifier between 0 to 0x7FF
	uint32_t ExtId;	//Extended identifier between 0 to 0x1FFFFFFF
	uint8_t IDE;	//Type of identifier used by this transmission: 0 - standard ID, 4 - extended ID
	uint8_t RTR;	//Type of frame for the message that will be transmitted: 0 - data frame, 2 - remote frame
	uint8_t DLC;	//length of the data field in bytes: 0 - 8
	uint8_t data[8];
} CAN_frameTx;

static const uint32_t INFINITE = 0xffffffff;

typedef enum
{
	NONE,
	ODD,
	EVEN
} EUartParity;

typedef enum
{
	ONE,
	TWO
} EUartStopBits;

#ifdef __cplusplus
enum class Parity { None = 0, Odd = 1, Even = 2 };
enum class StopBits { One = 0, Two = 1 };

/**
 * @brief Polarity for motor and encoder direction.
 */
enum class Polarity { 
	Normal = 0, //!< Normal polarity
	Reversed = 1 //!< Reversed polarity
};
#endif

typedef enum
{
	FLOW_CONTROL_NONE,
	FLOW_CONTROL_RTS_CTS
} EUartFlowControl;

/**
 * @brief Motor polarity.
 */
enum EMotorPolarity
{
	MOTOR_NORMAL, //!< Motor spin direction is normal.
	MOTOR_REVERSED, //!< Motor spin direction is reversed.
};

/**
 * @brief Encoder polarity.
 */
enum EEncoderPolarity
{
	ENCODER_NORMAL, //!< Encoder count direction is normal.
	ENCODER_REVERSED, //!< Encoder counts in the opposide direction.
};

// eg: BOOT_PARAM(disable_syslog, 1);
#define BOOT_PARAM(name,value) extern "C" int boot_##name = (value);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <functional>

typedef void (*UserDataFunc)(void*);
typedef std::function<void()> Handler;

inline void doNothing() {}

// IGPIO_int
enum class InterruptEdge { Falling, Rising, Both, None };
enum class CountingMode { Normal, Counting };

#endif

typedef unsigned char byte;

#endif /* __HTYPES_H__ */

/*! @} */
