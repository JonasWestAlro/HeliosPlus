#include "PPMReceiver.hpp"


static ControlReceiverCalibration standard_calibration = {
	/*HIGH*/	1881, 1891, 1892, 1892, 1892, 1883,
	/*LOW*/		1088, 1095, 1094, 1093, 1093, 1089
};

static uint8_t standard_channel_mapping[6] = {
		/*CHANNEL_THROTTLE:*/ 0, /*<--- these are the physical channels from the receiver*/
		/*CHANNEL_ROLL:    */ 2,
		/*CHANNEL_PITCH:   */ 1,
		/*CHANNEL_YAW:     */ 3,
		/*CHANNEL_AUX1:    */ 4,
		/*CHANNEL_AUX2:    */ 5
};
/*
static uint8_t standard_channel_mapping[6] = {
		/*CHANNEL_THROTTLE: 0,
		/*CHANNEL_ROLL:     1,
		/*CHANNEL_PITCH:    2,
		/*CHANNEL_YAW:      3,
		/*CHANNEL_AUX1:     4,
		/*CHANNEL_AUX2:     5
};*/

PPMReceiver::PPMReceiver(){
		memcpy(&calibration, &standard_calibration, sizeof(ControlReceiverCalibration));
		memcpy(channel_mapping, standard_channel_mapping, sizeof(standard_channel_mapping));

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

	sorted_channel_values.throttle = get_channel(CHANNEL_THROTTLE);
	sorted_channel_values.pitch    = get_channel(CHANNEL_PITCH);
	sorted_channel_values.roll     = get_channel(CHANNEL_ROLL);
	sorted_channel_values.yaw 	   = get_channel(CHANNEL_YAW);
	sorted_channel_values.aux1 	   = get_channel(CHANNEL_AUX1);
	sorted_channel_values.aux2     = get_channel(CHANNEL_AUX2);

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
		norm_channel = get_normalized_channel(channel_mapping[channel]);

		if(channel == CHANNEL_AUX1 || channel == CHANNEL_AUX2){
			if(norm_channel > 500) return 1000;
			else return -1000;
		}else
			return norm_channel;
	}

	return norm_channel;
}

void  PPMReceiver::get_all_channels(ControlReceiverValues& result){
	result = sorted_channel_values;
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
