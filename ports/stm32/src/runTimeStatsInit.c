#include "stm32f4xx.h"

#include "FreeRTOS.h"

//=============FOR RUN TIME STATS START =================================


volatile uint32_t tim_cnt = 0;

void TIM6_DAC_IRQHandler(void)
{
	//	extern int tim_cnt;

	tim_cnt++;

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

} /* TIM2_IRQHandler */

// to generate runtime stats
void tim_config(void)
{
	//sys_log("\r\nRUNTIME stats tim_config");
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

//	/* PCLK1 = HCLK/4 */
//	RCC_PCLK1Config(RCC_HCLK_Div4);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM6, ENABLE);

	tim_cnt = 0;
	TIM6->CNT = 0;
}

uint32_t get_tim(void)
{
	return (tim_cnt << 16) + TIM_GetCounter(TIM6);
}


//=============FOR RUN TIME STATS END ===================================
