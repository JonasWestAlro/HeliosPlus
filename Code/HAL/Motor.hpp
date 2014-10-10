#pragma once

#include "Framework.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#define MINSPEED 		0
#define MAXSPEED 		1000
#define OCOFFSET 		3277

class Motor : public HAL_Motor_I {

	public:
		Motor(){
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

			//Setup GPIOD to Output
			GPIO_InitTypeDef GPIODInitStruct;

			GPIODInitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIODInitStruct.GPIO_OType = GPIO_OType_PP;
			GPIODInitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
			GPIODInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIODInitStruct.GPIO_Speed = GPIO_Speed_100MHz;

			GPIO_Init(GPIOD, &GPIODInitStruct);

			GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
			GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
			GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
			GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

			//Setup GPIOA to Output
			GPIO_DeInit(GPIOA);
			GPIO_InitTypeDef GPIOAInitStruct;

			GPIOAInitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIOAInitStruct.GPIO_OType = GPIO_OType_PP;
			GPIOAInitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
			GPIOAInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIOAInitStruct.GPIO_Speed = GPIO_Speed_100MHz;

			GPIO_Init(GPIOA, &GPIOAInitStruct);

			GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);


			//Setup basic timer settings for timer 4 and 5
			TIM_TimeBaseInitTypeDef TimerBaseInitStruct;
			TIM_TimeBaseStructInit(&TimerBaseInitStruct);
			TIM_DeInit(TIM4);
			TIM_DeInit(TIM5);

			TimerBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
			TimerBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
			TimerBaseInitStruct.TIM_Period = 65535;
			TimerBaseInitStruct.TIM_Prescaler = 25;
			TIM_TimeBaseInit(TIM4, &TimerBaseInitStruct);
			TIM_TimeBaseInit(TIM5, &TimerBaseInitStruct);

			//Setup output compare mode settings for timer 4 and 5
			TIM_OCInitTypeDef TimerOCInitStruct;
			TIM_OCStructInit(&TimerOCInitStruct);

			TimerOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
			TimerOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
			TimerOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
			TimerOCInitStruct.TIM_Pulse = 0;
			TIM_OC1Init(TIM4, &TimerOCInitStruct);
			TIM_OC2Init(TIM4, &TimerOCInitStruct);
			TIM_OC3Init(TIM4, &TimerOCInitStruct);
			TIM_OC4Init(TIM4, &TimerOCInitStruct);
			TIM_OC1Init(TIM5, &TimerOCInitStruct);
			TIM_OC2Init(TIM5, &TimerOCInitStruct);
			TIM_OC3Init(TIM5, &TimerOCInitStruct);
			TIM_OC4Init(TIM5, &TimerOCInitStruct);

			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
			TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
			TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

			TIM_Cmd(TIM4, ENABLE);
			TIM_Cmd(TIM5, ENABLE);

			uint8_t i = 0;
			for(i;i<8;i++){
				*(CCR_Regs[i])=OCOFFSET;
			}

		}

	    virtual void set_motor_speed(MOTOR motor, uint16_t speed){

	    	uint8_t i = 0;
	    	for(i;i<8;i++){
	    		if(motor & 1<<i)
	    		{
	    			if(speed >= MINSPEED && speed <= MAXSPEED ){
	    				uint16_t setSpeed = (speed*OCOFFSET/1000.0)+OCOFFSET;
	    				*(CCR_Regs[i]) = setSpeed;
	    			}else{
	    				*(CCR_Regs[i]) = OCOFFSET;
	    			}
	    		}
	    	}

	    }

	private:
	    volatile uint32_t* CCR_Regs[8] = {	 &(TIM4->CCR1),
										 &(TIM4->CCR2),
										 &(TIM4->CCR3),
										 &(TIM4->CCR4),
										 &(TIM5->CCR1),
										 &(TIM5->CCR2),
										 &(TIM5->CCR3),
										 &(TIM5->CCR4)};

};
