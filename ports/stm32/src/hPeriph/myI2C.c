/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "stm32f4xx.h"
#include "myI2C.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "myUSART.h"
#include "myGPIO.h"

#define DEBUG_CATEGORY I2C
#include "debug.h"
#include "system.h"

#define MAX_TRANSMISSION_TIME_MS 20

typedef struct
{
	uint16_t pin;
	GPIO_TypeDef* port;
	uint32_t gpio_clock;
	uint8_t gpio_pin_source;
	uint8_t gpio_af;
} GPIO_config_t;

typedef struct
{
	uint8_t I2C_addr;
	uint8_t* I2C_data_buf;
	uint32_t I2C_data_buf_size;
	uint32_t I2C_data_pos;
	uint8_t I2C_direction;	//0 - TX , 1 - RX, 2 - TX and RX
	I2C_ERROR error_flag;
} communication_globals_t;

static uint8_t i2cNrFlag[3] = {0, 0, 0};

typedef struct
{
	GPIO_config_t sda;
	GPIO_config_t scl;

	//for I2C
	I2C_TypeDef* i2c;
	uint32_t i2c_clock;

	//for DMA
	// uint32_t DR_address;
	IRQn_Type I2C_x_EV_IRQn, I2C_x_ER_IRQn;

	xSemaphoreHandle myMutex;
	xSemaphoreHandle commSem;
	communication_globals_t myGlobals;

	uint32_t baudrate;
} I2C_config_t;

#define I2C_ENTRY(gpioSDA, pinSDA, gpioSCL, pinSCL, _i2c) { \
 	.sda = { GPIO_Pin_ ## pinSDA, gpioSDA, RCC_AHB1Periph_ ## gpioSDA, GPIO_PinSource ## pinSDA, GPIO_AF_ ## _i2c }, \
 	.scl = { GPIO_Pin_ ## pinSCL, gpioSCL, RCC_AHB1Periph_ ## gpioSCL, GPIO_PinSource ## pinSCL, GPIO_AF_ ## _i2c }, \
	.i2c = _i2c, \
	.i2c_clock = RCC_APB1Periph_ ## _i2c, \
	.I2C_x_EV_IRQn = _i2c ## _EV_IRQn, \
	.I2C_x_ER_IRQn = _i2c ## _ER_IRQn, \
	.myMutex = NULL, \
	.commSem = NULL, \
	.myGlobals = { 0, 0, 0, 0, 0, I2C_ERROR_NONE }, \
	.baudrate = 0 }

I2C_config_t i2cs[] =
{
#if BOARD(ROBOCORE)
	I2C_ENTRY(GPIOB, 9, GPIOB, 8, I2C1),
	I2C_ENTRY(GPIOF, 0, GPIOF, 1, I2C2),
	I2C_ENTRY(GPIOC, 9, GPIOA, 8, I2C3),
#elif BOARD(CORE2)
	I2C_ENTRY(GPIOF, 0, GPIOF, 1, I2C2),
	I2C_ENTRY(GPIOB, 9, GPIOB, 8, I2C1),
	I2C_ENTRY(GPIOC, 9, GPIOA, 8, I2C3),
#elif BOARD(CORE2MINI)
	I2C_ENTRY(GPIOC, 9, GPIOA, 8, I2C3),
#else
#  error no board
#endif
};

static inline I2C_ERROR myI2C_read_internal(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize, uint8_t noStart);
static inline I2C_ERROR myI2C_write_internal(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize);
static inline I2C_ERROR myI2C_waitForBusy(I2C_config_t* i2c);

// I2C_config_t* getI2C(uint8_t nr)
// {
// switch (nr)
// {
// case 1: return &myI2C1; break;
// case 2: return &myI2C2; break;
// case 3: return &myI2C3; break;
// default: ASSERT(0);
// }
// }

void myI2C_enableIRQ(I2C_config_t* i2c)
{
	I2C_ITConfig(i2c->i2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
}
void myI2C_disableIRQ(I2C_config_t* i2c)
{
	I2C_ITConfig(i2c->i2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, DISABLE);
}

void myI2C_reinit(I2C_config_t* i2c)
{
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphResetCmd(i2c->i2c_clock, ENABLE);
	RCC_APB1PeriphResetCmd(i2c->i2c_clock, DISABLE);

	// pclk1 / maxDivider / 2
	uint32_t minFreq = 84000000 / 4095 / 2;

	if (i2c->baudrate < minFreq)
	{
		i2c->baudrate = minFreq;
	}
	/* Configure I2C */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 1;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = i2c->baudrate;
	I2C_Init(i2c->i2c, &I2C_InitStructure);

	myI2C_disableIRQ(i2c);
	I2C_Cmd(i2c->i2c, ENABLE);
	// LOG("en");
}

void myI2C_init(uint8_t nr, uint32_t baudrate)
{
	I2C_config_t* i2c = &i2cs[nr];

	if (i2cNrFlag[nr])
		return;
	i2cNrFlag[nr] = 1;

	NVIC_InitTypeDef 	NVIC_InitStructure;

	vSemaphoreCreateBinary(i2c->commSem);
	i2c->myMutex = xSemaphoreCreateMutex();

	xSemaphoreTake(i2c->commSem, 0);

	//===================================================
	RCC_APB1PeriphClockCmd(i2c->i2c_clock, ENABLE);
	RCC_AHB1PeriphClockCmd(i2c->scl.gpio_clock | i2c->sda.gpio_clock, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	i2c->baudrate = baudrate;

	/* Configure and enable I2C event interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = i2c->I2C_x_EV_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = i2c->I2C_x_ER_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	myI2C_acquireGPIO(nr);

	myI2C_reinit(i2c);
}
void myI2C_deinit(uint8_t nr)
{
	I2C_config_t* i2c = &i2cs[nr];

	if (!i2cNrFlag[nr])
		return;

	// LOG("dein");
	vSemaphoreDelete(i2c->myMutex);
	vSemaphoreDelete(i2c->commSem);

	GPIO_InitTypeDef GPIO_InitStructure;

	myI2C_disableIRQ(i2c);
	I2C_Cmd(i2c->i2c, DISABLE);

	GPIO_InitStructure.GPIO_Pin = i2c->scl.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(i2c->scl.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = i2c->sda.pin;
	GPIO_Init(i2c->sda.port, &GPIO_InitStructure);

	i2cNrFlag[nr] = 0;
}
I2C_ERROR myI2C_changeBaudrate(uint8_t nr, uint32_t baudrate)
{
	I2C_config_t* i2c = &i2cs[nr];
	// I2C_ERROR result;

	if (xSemaphoreTake(i2c->myMutex, MAX_TRANSMISSION_TIME_MS * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		i2c->baudrate = baudrate;
		myI2C_reinit(i2c);

		xSemaphoreGive(i2c->myMutex);
		return I2C_ERROR_NONE;
	}
	else
	{
		return I2C_ERROR_TIMEOUT;
	}
}

I2C_ERROR myI2C_write(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize)
{
	I2C_config_t* i2c = &i2cs[nr];
	I2C_ERROR result;

	if (xSemaphoreTake(i2c->myMutex, MAX_TRANSMISSION_TIME_MS * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		result = myI2C_write_internal(nr, addr, dataBuf, bufSize);

		xSemaphoreGive(i2c->myMutex);
		return result;
	}
	else
	{
		return I2C_ERROR_TIMEOUT;
	}
}
I2C_ERROR myI2C_read(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize)
{
	I2C_config_t* i2c = &i2cs[nr];
	I2C_ERROR result;

	if (xSemaphoreTake(i2c->myMutex, MAX_TRANSMISSION_TIME_MS * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		result = myI2C_read_internal(nr, addr, dataBuf, bufSize, 0);

		xSemaphoreGive(i2c->myMutex);
		return result;
	}
	else
	{
		return I2C_ERROR_TIMEOUT;
	}
}
I2C_ERROR myI2C_read_write(uint8_t nr, uint8_t addr, uint8_t* dataBuf_TX, uint32_t bufSize_TX, uint8_t* dataBuf_RX, uint32_t bufSize_RX, uin32_t rxDelay)
{
	I2C_config_t* i2c = &i2cs[nr];
	I2C_ERROR result;

	if (xSemaphoreTake(i2c->myMutex, (MAX_TRANSMISSION_TIME_MS * 2) * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		result = myI2C_write_internal(nr, addr, dataBuf_TX, bufSize_TX);
		if (result != I2C_ERROR_NONE)
		{
			xSemaphoreGive(i2c->myMutex);
			return result;
		}

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = i2c->scl.pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(i2c->scl.port, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = i2c->sda.pin;
		GPIO_Init(i2c->sda.port, &GPIO_InitStructure);

		GPIO_ResetBits(i2c->scl.port, i2c->scl.pin);
		sys_delay_us(rxDelay);
		GPIO_SetBits(i2c->scl.port, i2c->scl.pin);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

		GPIO_InitStructure.GPIO_Pin = i2c->scl.pin;
		GPIO_Init(i2c->scl.port, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = i2c->sda.pin;
		GPIO_Init(i2c->sda.port, &GPIO_InitStructure);

		result = myI2C_read_internal(nr, addr, dataBuf_RX, bufSize_RX, 0);

		xSemaphoreGive(i2c->myMutex);
		return result;
	}
	else
	{
		return I2C_ERROR_TIMEOUT;
	}
}

void myI2C_acquireGPIO(uint8_t nr)
{
	I2C_config_t* i2c = &i2cs[nr];

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = i2c->scl.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(i2c->scl.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = i2c->sda.pin;
	GPIO_Init(i2c->sda.port, &GPIO_InitStructure);

	GPIO_PinAFConfig(i2c->scl.port, i2c->scl.gpio_pin_source, i2c->scl.gpio_af);
	GPIO_PinAFConfig(i2c->sda.port, i2c->sda.gpio_pin_source, i2c->sda.gpio_af);
}
void myI2C_releaseGPIO(uint8_t nr)
{
	I2C_config_t* i2c = &i2cs[nr];

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = i2c->scl.pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(i2c->scl.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = i2c->sda.pin;
	GPIO_Init(i2c->sda.port, &GPIO_InitStructure);
}

static inline I2C_ERROR myI2C_read_internal(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize, uint8_t noStart)
{
	I2C_config_t* i2c = &i2cs[nr];
	// I2C_ERROR result;

	if (i2c->i2c->SR2 & I2C_SR2_BUSY)
		myI2C_reinit(i2c);

	i2c->myGlobals.I2C_data_buf = dataBuf;
	i2c->myGlobals.I2C_addr = addr << 1;
	i2c->myGlobals.I2C_data_buf_size = bufSize;
	i2c->myGlobals.I2C_direction = 1;
	i2c->myGlobals.error_flag = I2C_ERROR_NONE;

	if (noStart == 0)
	{
		I2C_GenerateSTART(i2c->i2c, ENABLE);
	}
	myI2C_enableIRQ(i2c);

	if (xSemaphoreTake(i2c->commSem, MAX_TRANSMISSION_TIME_MS * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		myI2C_waitForBusy(i2c); // ignore result

		// I2C_ITConfig(i2c->i2c, I2C_IT_EVT, ENABLE);

		return i2c->myGlobals.error_flag;
	}
	else
	{
		// LOG("reto");
		myI2C_reinit(i2c);
		// sys_log("I2C RX Timeout, nr=%d\r\n", nr);
		return I2C_ERROR_TIMEOUT;
	}

}
static inline I2C_ERROR myI2C_write_internal(uint8_t nr, uint8_t addr, uint8_t* dataBuf, uint32_t bufSize)
{
	I2C_config_t* i2c = &i2cs[nr];

	if (i2c->i2c->SR2 & I2C_SR2_BUSY)
		myI2C_reinit(i2c);

	i2c->myGlobals.I2C_data_buf = dataBuf;
	i2c->myGlobals.I2C_addr = addr << 1;
	i2c->myGlobals.I2C_data_buf_size = bufSize;
	i2c->myGlobals.I2C_direction = 0;
	i2c->myGlobals.error_flag = I2C_ERROR_NONE;
	// LOG("gest");
	I2C_GenerateSTART(i2c->i2c, ENABLE);
	myI2C_enableIRQ(i2c);

	if (xSemaphoreTake(i2c->commSem, MAX_TRANSMISSION_TIME_MS * portTICK_MS_TO_TICKS) == pdTRUE)
	{
		myI2C_waitForBusy(i2c);
		// uint32_t reg_SR1, reg_SR2;
		// reg_SR1 = i2c->i2c->SR1;
		// reg_SR2 = i2c->i2c->SR2;
		// fail_log("%08x %08x\r\n", reg_SR1, reg_SR2);
		return i2c->myGlobals.error_flag;
	}
	else
	{
		// LOG("wrto");
		myI2C_reinit(i2c);
		// sys_log("I2C TX Timeout, nr=%d\r\n", nr);
		return I2C_ERROR_TIMEOUT;
	}
}
static inline I2C_ERROR myI2C_waitForBusy(I2C_config_t* i2c)
{
	uint32_t startTime = xTaskGetTickCount();
	while (I2C_GetFlagStatus(i2c->i2c, I2C_FLAG_BUSY))
	{
		uint32_t reg_SR1, reg_SR2;
		reg_SR1 = i2c->i2c->SR1;
		reg_SR2 = i2c->i2c->SR2;
		// fail_log("s SR1 0x%04x SR2 0x%04x 0x%08x\r\n", reg_SR1, reg_SR2, i2c->i2c->CR1);
		sys_delay_ms(1);
		if (i2c->i2c->SR1 & I2C_SR1_BERR)
		{
			// LOG("res\r\n");
			I2C_GenerateSTOP(i2c->i2c, ENABLE);
			myI2C_reinit(i2c);
		}
		if ((xTaskGetTickCount() - startTime) * portTICK_MS_TO_TICKS >= MAX_TRANSMISSION_TIME_MS)
			return I2C_ERROR_TIMEOUT;
	}
	return I2C_ERROR_NONE;
}

void i2c_x_ev_funct(uint8_t nr)
{
	I2C_config_t* i2c = &i2cs[nr];
	/*
	 * @arg I2C_EVENT_MASTER_MODE_SELECT: EV5
	 * @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: EV6
	 * @arg I2C_EVENT_MASTER_BYTE_TRANSMITTING: EV8
	 * @arg I2C_EVENT_MASTER_BYTE_TRANSMITTED: EV8_2
	 */
	signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	volatile uint32_t reg_SR1;
	volatile uint32_t reg_SR2;
	volatile uint32_t I2CFlagStatus;

	/* Read the I2Cx status register */
	reg_SR1 = i2c->i2c->SR1;
	reg_SR2 = i2c->i2c->SR2;

	reg_SR2 = reg_SR2 << 16;

	/* Get the last event value from I2C status register */
	I2CFlagStatus = (reg_SR1 | reg_SR2) & (uint32_t)0x00FFFFFF;

	uint8_t inByte;
	if (i2c->i2c->SR1 & I2C_SR1_RXNE)
	{
		inByte = i2c->i2c->DR;
	}

	/*----------------------------------------*/
	/*------------- If SB event --------------*/
	// EV5
	if ((I2CFlagStatus & I2C_EVENT_MASTER_MODE_SELECT) == I2C_EVENT_MASTER_MODE_SELECT)
	{
		// fail_log("I2C_EVENT_MASTER_MODE_SELECT\r\n");
		// LOGIRQ("sb");
		if (i2c->myGlobals.I2C_direction == 1)
		{
			/* Send 7 bit address */
			I2C_Send7bitAddress(i2c->i2c, i2c->myGlobals.I2C_addr, I2C_Direction_Receiver);
		}
		else
		{
			//0 albo 2

			/* Send 7 bit address */
			I2C_Send7bitAddress(i2c->i2c, i2c->myGlobals.I2C_addr, I2C_Direction_Transmitter);
		}
		// return;
	}

	/*----------------------------------------*/
	/*------------- If ADDR event ------------*/
	// EV6
	if ((I2CFlagStatus & I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
	{
		// LOGIRQ("mdtx");
		i2c->myGlobals.I2C_data_pos = 0;
		if (i2c->myGlobals.I2C_direction == 0 || i2c->myGlobals.I2C_direction == 2)
		{
			if (i2c->myGlobals.I2C_data_buf_size > 0)
			{
				i2c->i2c->DR = i2c->myGlobals.I2C_data_buf[i2c->myGlobals.I2C_data_pos];
				i2c->myGlobals.I2C_data_pos++;
			}
			else
			{
				// LOGIRQ("st");
				I2C_GenerateSTOP(i2c->i2c, ENABLE);
				myI2C_disableIRQ(i2c);
				xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
				goto end;
			}
		}
		// return;
	}
	if ((I2CFlagStatus & I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
	{
		// LOGIRQ("mdrx");
		if (i2c->myGlobals.I2C_direction == 1) // || i2c->myGlobals.I2C_direction == 2 ) {
		{
			if (i2c->myGlobals.I2C_data_buf_size == 1)
			{
				I2C_AcknowledgeConfig(i2c->i2c, DISABLE); //see reference manual s. 583
				I2C_GenerateSTOP(i2c->i2c, ENABLE);
			}
			else
			{
				I2C_AcknowledgeConfig(i2c->i2c, ENABLE);
			}
			reg_SR2 = i2c->i2c->CR2;
			i2c->myGlobals.I2C_data_pos = 0;

		}
		// return;
	}

	// EV7
	if ((I2CFlagStatus & I2C_EVENT_MASTER_BYTE_RECEIVED) == I2C_EVENT_MASTER_BYTE_RECEIVED)
	{
		if (i2c->myGlobals.I2C_direction == 1 || i2c->myGlobals.I2C_direction == 2)
		{
			i2c->myGlobals.I2C_data_buf[i2c->myGlobals.I2C_data_pos] = inByte;

			i2c->myGlobals.I2C_data_pos++;
			if (i2c->myGlobals.I2C_data_pos == i2c->myGlobals.I2C_data_buf_size - 1)
			{
				I2C_AcknowledgeConfig(i2c->i2c, DISABLE);

				// We should generate STOP condition only if there was no overrun
				// (data not read from DR and new data arrived).
				// In this case we should set NACK, read upcoming data and send
				// STOP condition after last byte.
				if (!(reg_SR1 & I2C_SR1_BTF))
				{
					I2C_GenerateSTOP(i2c->i2c, ENABLE);
				}
			}
			else if (i2c->myGlobals.I2C_data_pos == i2c->myGlobals.I2C_data_buf_size)
			{
				// If there was an overrun, read data and generate STOP condition.
				if (reg_SR1 & I2C_SR1_BTF)
				{
					volatile uint8_t d = i2c->i2c->DR;
					I2C_GenerateSTOP(i2c->i2c, ENABLE);
				}
				if (i2c->myGlobals.I2C_direction == 1)
				{
					myI2C_disableIRQ(i2c);
					// LOGIRQ("gv0 %d", xHigherPriorityTaskWoken);
					xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
					goto end;
				}
			}
		}
		// return;
	}

	// EV8
	if ((I2CFlagStatus & I2C_EVENT_MASTER_BYTE_TRANSMITTING) == I2C_EVENT_MASTER_BYTE_TRANSMITTING)
	{
		// LOGIRQ("tring %d %d", i2c->myGlobals.I2C_data_pos, i2c->myGlobals.I2C_data_buf_size);
		if (i2c->myGlobals.I2C_data_pos < i2c->myGlobals.I2C_data_buf_size)
		{
			if (i2c->myGlobals.I2C_direction == 0 || i2c->myGlobals.I2C_direction == 2)
			{
				i2c->i2c->DR = i2c->myGlobals.I2C_data_buf[i2c->myGlobals.I2C_data_pos];
				i2c->myGlobals.I2C_data_pos++;
			}
		}
		else
		{
			I2C_GenerateSTOP(i2c->i2c, ENABLE);
			myI2C_disableIRQ(i2c);
			xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
			// LOGIRQ("gv1 %d", xHigherPriorityTaskWoken);
			goto end;
		}
		// return;
	}

	/// EV8_2
	if ((I2CFlagStatus & I2C_EVENT_MASTER_BYTE_TRANSMITTED) == I2C_EVENT_MASTER_BYTE_TRANSMITTED)
	{
		// LOGIRQ("tred");

		myI2C_disableIRQ(i2c);
		if (i2c->myGlobals.I2C_direction == 0)
		{
			I2C_GenerateSTOP(i2c->i2c, ENABLE);
		}

		if (i2c->myGlobals.I2C_direction == 2)
		{
			I2C_GenerateSTART(i2c->i2c, ENABLE);
		}

		// LOGIRQ("gv2 %d", xHigherPriorityTaskWoken);
		xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
		goto end;
	}

end:
	if (xHigherPriorityTaskWoken == pdTRUE)
		taskYIELD();
}
void i2c_x_er_funct(uint8_t nr)
{
	I2C_config_t* i2c = &i2cs[nr];

	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	volatile uint32_t reg_SR1;
	volatile uint32_t reg_SR2;

	/* Read the I2Cx status register */
	reg_SR1 = i2c->i2c->SR1;
	reg_SR2 = i2c->i2c->SR2;

	if (reg_SR1 & I2C_SR1_AF)
	{
		i2c->i2c->SR1 &= ~I2C_SR1_AF;
		// if (i2c->myGlobals.I2C_direction == 0)
		{
			I2C_GenerateSTOP(i2c->i2c, ENABLE);
		}
		i2c->myGlobals.error_flag = I2C_ERROR_NACK;
		myI2C_disableIRQ(i2c);
		// LOGIRQ("gv3 %d", xHigherPriorityTaskWoken);
		xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
	}
	if (reg_SR1 & I2C_SR1_BERR)
	{
		i2c->i2c->SR1 &= ~I2C_SR1_BERR;
		i2c->myGlobals.error_flag = I2C_ERROR_BUS_ERROR;
		myI2C_disableIRQ(i2c);
		// LOGIRQ("gv4 %d", xHigherPriorityTaskWoken);
		xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
	}
	if (reg_SR1 & I2C_SR1_ARLO)
	{
		i2c->i2c->SR1 &= ~I2C_SR1_ARLO;
		i2c->myGlobals.error_flag = I2C_ERROR_ARBITATION_LOST;
		myI2C_disableIRQ(i2c);
		// LOGIRQ("gv5 %d", xHigherPriorityTaskWoken);
		xSemaphoreGiveFromISR(i2c->commSem, &xHigherPriorityTaskWoken);
	}
	// if (reg_SR1 & I2C_SR1_OVR)
	// {
	// fail_log("OVR\r\n");
	// i2c->i2c->SR1 &= ~I2C_SR1_OVR;
	// }
	if (xHigherPriorityTaskWoken == pdTRUE)
		taskYIELD();
}

#define I2C_IRQ(_i2c, _id) \
	void _i2c ## _EV_IRQHandler(void) { i2c_x_ev_funct(_id); } \
	void _i2c ## _ER_IRQHandler(void) { i2c_x_er_funct(_id); }

#if BOARD(ROBOCORE)
I2C_IRQ(I2C1, MYI2C_EXT)
I2C_IRQ(I2C2, MYI2C_SENS1)
I2C_IRQ(I2C3, MYI2C_SENS2)
#elif BOARD(CORE2)
I2C_IRQ(I2C2, MYI2C_EXT)
I2C_IRQ(I2C1, MYI2C_SENS1)
I2C_IRQ(I2C3, MYI2C_SENS2)
#elif BOARD(CORE2MINI)
I2C_IRQ(I2C3, MYI2C_SENS1)
#else
#  error no board
#endif
