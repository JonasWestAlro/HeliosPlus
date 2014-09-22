#pragma once

#include "Framework.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

class Multiplexer : public HAL_Multiplexer_I{

	public:
	Multiplexer(){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		//Setup GPIOD to Output
		GPIO_InitTypeDef GPIOInitStruct;

		GPIOInitStruct.GPIO_Mode 	= GPIO_Mode_OUT;
		GPIOInitStruct.GPIO_OType 	= GPIO_OType_PP;
		GPIOInitStruct.GPIO_Pin 	= GPIO_Pin_1;
		GPIOInitStruct.GPIO_PuPd 	= GPIO_PuPd_DOWN;
		GPIOInitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;

		GPIO_Init(GPIOE, &GPIOInitStruct);
		set_channel(FLIGHTCONTROLLER);

		}

	virtual void set_channel(MULTIPLEXER_CHANNEL channel){

		if(channel == FLIGHTCONTROLLER){
			GPIO_SetBits(GPIOE,GPIO_Pin_1);
		}

		if(channel == APPLICATIONPROCCESOR){
			GPIO_ResetBits(GPIOE,GPIO_Pin_1);
		}
	};

	private:

};
