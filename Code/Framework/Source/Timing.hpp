#pragma once
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

class Timing;
extern Timing Time;

class Timing{
public:
	Timing(){
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

	unsigned int get_timestamp(void){
		return TIM_GetCounter(TIM2);
	}

	unsigned int get_time_since_us(unsigned int timestamp){
		return get_timestamp() - timestamp;
	}

	float get_time_since_ms(unsigned int timestamp){
		return ((float)get_time_since_us(timestamp))/1000.0f;
	}

	float get_time_since_sec(unsigned int timestamp){
		return ((float)get_time_since_us(timestamp))/1000000.0f;
	}

	void delay_ms(uint32_t ms){
		uint32_t timestamp = get_timestamp();
		while(get_time_since_ms(timestamp) < ms);
	}

	void delay_us(uint32_t us){
		uint32_t timestamp = get_timestamp();
		while(get_time_since_us(timestamp) < us);
	}

};
