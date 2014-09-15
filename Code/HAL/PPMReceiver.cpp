#include "PPMReceiver.hpp"


static ControlReceiverCalibration standard_calibration = {
	/*HIGH*/	1881, 1891, 1892, 1892, 1892, 1883,
	/*LOW*/		1088, 1095, 1094, 1093, 1093, 1089
};

static ControlReceiverChannel standard_channel_mapping[6] = {
		/*Channel 1:*/ CHANNEL_THROTTLE,
		/*Channel 2:*/ CHANNEL_ROLL,
		/*Channel 3:*/ CHANNEL_PITCH,
		/*Channel 4:*/ CHANNEL_YAW,
		/*Channel 5:*/ CHANNEL_SWITCH,
		/*Channel 6:*/ CHANNEL_CLICK,
};

PPMReceiver::PPMReceiver(){
		std::memcpy(&calibration, &standard_calibration, sizeof(ControlReceiverCalibration));
		std::memcpy(channel_mapping, standard_channel_mapping, sizeof(standard_channel_mapping));

		channel1.setup( GPM_MODE_CONTINOUS_RUNNING,

						TIM3, 						//TIMx
						RCC_APB1Periph_TIM3,
						APB1,
						TIM_Channel_3,				//Master Channel
						TIM_Channel_4, 				//Slave Channel
						84, 						//Prescaler

						//GPIO SPECS
						GPIOB,						//GPIO-port
						RCC_AHB1Periph_GPIOB,		//GPIO_Periph
						GPIO_Pin_0,					//GPIO_Pin
						GPIO_PinSource0,			//GPIO_Pin Source
						GPIO_AF_TIM3			    //example: GPIO_AF_USART3
		);

		channel2.setup( 		GPM_MODE_CONTINOUS_RUNNING,

				TIM1, 						//TIMx
				RCC_APB2Periph_TIM1,
				APB2,
				TIM_Channel_1,				//Master Channel
				TIM_Channel_2, 				//Slave Channel
				168, 						//Prescaler

				//GPIO SPECS
				GPIOE,						//GPIO-port
				RCC_AHB1Periph_GPIOE,		//GPIO_Periph
				GPIO_Pin_9,					//GPIO_Pin
				GPIO_PinSource9,			//GPIO_Pin Source
				GPIO_AF_TIM1			    //example: GPIO_AF_USART3
		);

		channel3.setup( 		GPM_MODE_CONTINOUS_RUNNING,

				TIM1, 						//TIMx
				RCC_APB2Periph_TIM1,
				APB2,
				TIM_Channel_3,				//Master Channel
				TIM_Channel_4, 				//Slave Channel
				168, 						//Prescaler

				//GPIO SPECS
				GPIOE,						//GPIO-port
				RCC_AHB1Periph_GPIOE,		//GPIO_Periph
				GPIO_Pin_13,					//GPIO_Pin
				GPIO_PinSource13,			//GPIO_Pin Source
				GPIO_AF_TIM1			    //example: GPIO_AF_USART3
		);

		channel4.setup( 		GPM_MODE_CONTINOUS_RUNNING,

				TIM8, 						//TIMx
				RCC_APB2Periph_TIM8,
				APB2,
				TIM_Channel_1,				//Master Channel
				TIM_Channel_2, 				//Slave Channel
				168, 						//Prescaler

				//GPIO SPECS
				GPIOC,						//GPIO-port
				RCC_AHB1Periph_GPIOC,		//GPIO_Periph
				GPIO_Pin_6,					//GPIO_Pin
				GPIO_PinSource6,			//GPIO_Pin Source
				GPIO_AF_TIM8			    //example: GPIO_AF_USART3
		);

		channel5.setup( 		GPM_MODE_CONTINOUS_RUNNING,

				TIM8, 						//TIMx
				RCC_APB2Periph_TIM8,
				APB2,
				TIM_Channel_3,				//Master Channel
				TIM_Channel_4, 				//Slave Channel
				168, 						//Prescaler

				//GPIO SPECS
				GPIOC,						//GPIO-port
				RCC_AHB1Periph_GPIOC,		//GPIO_Periph
				GPIO_Pin_8,					//GPIO_Pin
				GPIO_PinSource8,			//GPIO_Pin Source
				GPIO_AF_TIM8			    //example: GPIO_AF_USART3
		);

		channel6.setup( 		GPM_MODE_CONTINOUS_RUNNING,

				TIM3, 						//TIMx
				RCC_APB1Periph_TIM3,
				APB1,
				TIM_Channel_1,				//Master Channel
				TIM_Channel_2, 				//Slave Channel
				84, 						//Prescaler

				//GPIO SPECS
				GPIOB,						//GPIO-port
				RCC_AHB1Periph_GPIOB,		//GPIO_Periph
				GPIO_Pin_4,					//GPIO_Pin
				GPIO_PinSource4,			//GPIO_Pin Source
				GPIO_AF_TIM3			    //example: GPIO_AF_USART3
		);


		//Start all the measurements:
		channel1.start();
		channel2.start();
		channel3.start();
		channel4.start();
		channel5.start();
		channel6.start();

	}

void PPMReceiver::execute(void){
	uint8_t i;
	uint16_t channel_val;

	for(i=0; i<6; i++){
		channel_val = channel_table[i]->get_pulse_lenght();

		if(channel_val < PLM_RECEIVER_MAX_PULSE &&
		   channel_val > PLM_RECEIVER_MIN_PULSE ){
			//Update channel:
			channel_values[i] = channel_val;
			last_update_timestamp[i] = Time.get_timestamp();
		}
	}

	update_status();
	if(calibrating) calibrate();
}

uint8_t PPMReceiver::get_status(void){
	return status;
}

float PPMReceiver::get_channel(ControlReceiverChannel channel){
	uint8_t i;
	float norm_channel;

	for(i=0; i<6; i++){
		if(channel == channel_mapping[i]){
			//here "i" is real channel number
			norm_channel = get_normalized_channel(i);

			if(channel == CHANNEL_SWITCH || channel == CHANNEL_CLICK){
				if(norm_channel > 500) return 1000;
				else return -1000;
			}else
				return norm_channel;
		}
	}

	return 0;
}

void 	 PPMReceiver::start_calibration(void){}
void 	 PPMReceiver::stop_calibration(void){}
void 	 PPMReceiver::get_calibration(ControlReceiverCalibration* new_calibration){}
void 	 PPMReceiver::set_calibration(ControlReceiverCalibration* c){}

void 	 PPMReceiver::update_status(){}
void 	 PPMReceiver::calibrate(){};

float 	 PPMReceiver::get_normalized_channel(uint8_t channel){
	float low = calibration.low[channel];
	float high = calibration.high[channel];
	float value = channel_values[channel] ;
	float result = 0;

	if(high == 0 || low == 0) return value;

	if(value < low) value = low;

	result = (((value - low) / (high-low)) * 2000) - 1000;

	if(result > 1000) result = 1000;
	if(result < -1000) result = -1000;

	return result;
}
