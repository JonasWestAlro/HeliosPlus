#pragma once

#include "Framework.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

class HC_SR40 : public HAL_Altitude_I{

	public:
	HC_SR40(){
		  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		  TIM_OCInitTypeDef  	   TIM_OCInitStructure;
		  GPIO_InitTypeDef 		   GPIO_InitStructure;
		  TIM_ICInitTypeDef  	   TIM_ICInitStructure;

		  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //TIM12

		  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
		  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN ;
		  GPIO_Init(GPIOB, &GPIO_InitStructure);

		  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15 ;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		  GPIO_Init(GPIOB, &GPIO_InitStructure) ;

		  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
		  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);

		  /* Time base configuration */
		  TIM_TimeBaseStructure.TIM_Period = 60000;
		  TIM_TimeBaseStructure.TIM_Prescaler = 84;
		  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		  TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);


		  //Init Input Capture
		  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2 ;
		  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
		  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI ;
		  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
		  TIM_ICInitStructure.TIM_ICFilter    = 0x0 ;

		  TIM_ICInit(TIM12, &TIM_ICInitStructure);

		  //STEP 3:
		  //TIM_SelectInputTrigger(TIM12, TIM_TS_TI2FP2);

		  /* PWM1 Mode configuration: Channel1 */
		  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		  TIM_OCInitStructure.TIM_Pulse = 20;
		  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

		  TIM_OC1Init(TIM12, &TIM_OCInitStructure);
		  TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
		 // TIM_SelectOnePulseMode(TIM12, TIM_OPMode_Single);

		  /* TIM12 enable counter */
		  TIM_Cmd(TIM12, ENABLE);

		  //TIM_SelectSlaveMode(TIM12, TIM_SlaveMode_Trigger);
	}

	virtual uint8_t data_available(void){
		return TIM_GetFlagStatus(TIM12, TIM_FLAG_CC2);
		return 1;
	};

	virtual float get_altitude(void){
		float returnVal;
		static uint8_t orderTrig;
		static uint32_t timestamp;
		static float lastReturnVal;

		if(data_available()){
			returnVal = (TIM_GetCapture2(TIM12) - 480.0) / 48.0;
			orderTrig = 1;
		}else{
			returnVal = lastReturnVal;
		}

		if(orderTrig){
			if(Time.get_time_since_ms(timestamp) > 10){
				sonar_trig();
				timestamp = Time.get_timestamp();
				orderTrig = 0;
			}
		}

		lastReturnVal = returnVal;
		return returnVal;
	};

	virtual uint8_t get_status(void){
		float altitude = get_altitude();
		if(altitude <= 0 || altitude > 250) return STATUS_NOTOK;
		else return STATUS_OK;
	};

	virtual void set_offset(float offset_){
		offset = offset_;
	};

	private:
		float offset = 0;

	void sonar_trig(){
			 TIM_Cmd(TIM12, DISABLE);
			 TIM_SetCounter(TIM12, 0);
			 TIM_Cmd(TIM12, ENABLE);
	}

};
