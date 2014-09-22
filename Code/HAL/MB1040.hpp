#pragma once

#include "Framework.hpp"

class MB1040 : public HAL_Altitude_I{

	public:
	MB1040(){
		pulse_measure.setup(
				GPM_MODE_RESET_ON_FALLING,
				TIM12, 						//TIMx
				RCC_APB1Periph_TIM12,
				APB1,
				TIM_Channel_1,				//Master Channel
				TIM_Channel_2, 				//Slave Channel
				84, 						//Prescaler

				//GPIO SPECS
				GPIOB,						//GPIO-port
				RCC_AHB1Periph_GPIOB,		//GPIO_Periph
				GPIO_Pin_14,				//GPIO_Pin
				GPIO_PinSource14,				//GPIO_Pin Source
				GPIO_AF_TIM12			    //example: GPIO_AF_USART3
		);

		pulse_measure.start();
	}

	virtual uint8_t data_available(void){
		//TODO-JWA: Implement this properly.
		return 1;
	};

	virtual float get_altitude(void){
		return ((((float)pulse_measure.get_pulse_lenght())/ 147.0) * 2.54) + offset;
	};

	virtual uint8_t get_status(void){
		if(get_altitude() <= 0) return STATUS_NOTOK;
		else return STATUS_OK;
	};

	virtual void set_offset(float offset_){
		offset = offset_;
	};

	private:
		GenericPulseMeasurement pulse_measure;
		float offset = 0;
};
