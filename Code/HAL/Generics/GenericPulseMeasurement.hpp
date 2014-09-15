#pragma once

#include "Framework.hpp"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define APB1 1
#define APB2 2

#define GPM_MODE_RESET_ON_FALLING 	0
#define GPM_MODE_CONTINOUS_RUNNING 	1

class GenericPulseMeasurement{
public:
	GenericPulseMeasurement(){


	}

	void setup(	uint8_t GPM_Mode_,

				TIM_TypeDef* TIMx_,  				//example: TIM12
				uint32_t TIM_Periph_,
				uint32_t TIM_APB_,
				uint16_t MasterChannel_,
				uint16_t SlaveChannel_,

				uint32_t TIM_Prescaler_, 			//example: 84

				//GPIO SPECS
				GPIO_TypeDef* 	GPIO_Port_,			//example: GPIOB
				uint32_t 		GPIO_Periph_,		//example: RCC_AHB1Periph_GPIOB
				uint32_t 		GPIO_Pin_,			//example: GPIO_Pin_5
				uint8_t 		PinSource_,			//example: GPIO_PinSource10
				uint8_t 		GPIO_AF_			//example: GPIO_AF_USART3
	){
		GPM_Mode = GPM_Mode_;
		TIMx = TIMx_;
		TIM_Periph = TIM_Periph_;
		TIM_APB = TIM_APB_;
		MasterChannel = MasterChannel_;
		SlaveChannel = SlaveChannel_;
		TIM_Prescaler = TIM_Prescaler_;
		GPIO_Port = GPIO_Port_;
		GPIO_Periph = GPIO_Periph_;
		GPIO_Pin = GPIO_Pin_;
		PinSource = PinSource_;			//example: GPIO_PinSource10
		GPIO_AF = GPIO_AF_;
	}

	void start(){
		//Setup structs.
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef  		 TIM_ICInitStructure_1;
		TIM_ICInitTypeDef  		 TIM_ICInitStructure_2;

		/*------------ GPIO SETUP -------------*/
		GPIO_InitTypeDef 		   GPIO_InitStructure;
		if(this->TIM_APB == APB1)
			 RCC_APB1PeriphClockCmd(this->TIM_Periph, ENABLE);
		else RCC_APB2PeriphClockCmd(this->TIM_Periph, ENABLE);

		RCC_AHB1PeriphClockCmd(	this->GPIO_Periph, ENABLE);

		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin 	= this->GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
		GPIO_Init(this->GPIO_Port, &GPIO_InitStructure);

		GPIO_PinAFConfig(this->GPIO_Port, this->PinSource, this->GPIO_AF);

		/*----------  TIMER SETUP --------------*/
		if(this->GPM_Mode == GPM_MODE_RESET_ON_FALLING){
			TIM_SelectInputTrigger(this->TIMx, TIM_TS_TI1FP1);
			TIM_SelectSlaveMode(this->TIMx, TIM_SlaveMode_Reset);
		}

		//Init Timer:
		TIM_TimeBaseStructure.TIM_Prescaler = this->TIM_Prescaler ;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF ;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseInit(this->TIMx, &TIM_TimeBaseStructure);

		//Enable input capture on Master channel:
		TIM_ICInitStructure_1.TIM_Channel =  this->MasterChannel;
		TIM_ICInitStructure_1.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure_1.TIM_ICFilter = 0xF;
		TIM_ICInitStructure_1.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure_1.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInit(this->TIMx, &TIM_ICInitStructure_1);

		//Enable input capture on slave channel:
		TIM_ICInitStructure_2.TIM_Channel =  this->SlaveChannel;
		TIM_ICInitStructure_2.TIM_ICSelection = TIM_ICSelection_IndirectTI;
		TIM_ICInitStructure_2.TIM_ICFilter = 0xF;
		TIM_ICInitStructure_2.TIM_ICPolarity = TIM_ICPolarity_Falling;
		TIM_ICInitStructure_2.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInit( this->TIMx, &TIM_ICInitStructure_2);

		TIM_Cmd(this->TIMx, ENABLE);
	}

	uint16_t get_pulse_lenght(){
		uint16_t risingchannel = 0;
		uint16_t fallingchannel = 0;


		if(this->GPM_Mode == GPM_MODE_RESET_ON_FALLING){
			return get_slave_channel();
		}

		if(this->GPM_Mode == GPM_MODE_CONTINOUS_RUNNING){
			risingchannel = get_master_channel();
			fallingchannel = get_slave_channel();

			return fallingchannel - risingchannel;
		}

		return 0;
	}

	uint16_t get_slave_channel(){
		switch(this->SlaveChannel){
			case TIM_Channel_1:
				return TIM_GetCapture1(this->TIMx);
			case TIM_Channel_2:
				return TIM_GetCapture2(this->TIMx);
			case TIM_Channel_3:
				return TIM_GetCapture3(this->TIMx);
			case TIM_Channel_4:
				return TIM_GetCapture4(this->TIMx);
			default:
				return 0;
		}
	}
	uint16_t get_master_channel(){
		switch(this->MasterChannel){
			case TIM_Channel_1:
				return TIM_GetCapture1(this->TIMx);
			case TIM_Channel_2:
				return TIM_GetCapture2(this->TIMx);
			case TIM_Channel_3:
				return TIM_GetCapture3(this->TIMx);
			case TIM_Channel_4:
				return TIM_GetCapture4(this->TIMx);
			default:
				return 0;
		}
	}

private:
	uint8_t GPM_Mode;

	TIM_TypeDef* TIMx;  				//example: TIM12
	uint32_t TIM_Periph;
	uint32_t TIM_APB;
	uint16_t MasterChannel;
	uint16_t SlaveChannel;

	uint32_t TIM_Prescaler; 			//example: 84

	//GPIO SPECS
	GPIO_TypeDef* 	GPIO_Port;			//example: GPIOB
	uint32_t 		GPIO_Periph;		//example: RCC_AHB1Periph_GPIOB
	uint32_t 		GPIO_Pin;			//example: GPIO_Pin_5
	uint8_t 		PinSource;			//example: GPIO_PinSource10
	uint8_t 		GPIO_AF;			//example: GPIO_AF_USART3
};
