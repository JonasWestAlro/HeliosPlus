#include "timerTest.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void Framework_Time_Init(void){
	//Init Timer 2:

	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
	  TIM_TimeBaseStructure.TIM_Prescaler = 84;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	  TIM_Cmd(TIM2, ENABLE);
}

uint32_t Framework_GetTimeStamp(void){
	return TIM_GetCounter(TIM2);
}
