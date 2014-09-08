#include "Communication.hpp"

#include "Globals.hpp"

void Communication::init(){

	// m_UART.init(GENERIC_UART_TYPE_USART3_PB10_PB11, 230400);

	//Setup Multiplexer
	/*GPIO_InitTypeDef GPIOInitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIOInitStruct);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);*/

}

void Communication::run(void){
	init();
	Message msg;
	while(1)
	{
		while(messenger.try_receive(&msg)){
			handle_message(msg);
		}

		//Testing interface:
		attitude.Pitch = 10;
		attitude.receive();

		//Testing message breoadcast:
		messenger.broadcast(CALIBRATE_GYROSCOPE);


		messenger.broadcast(Message(CALIBRATE_ACCELEROMETER));


		messenger.broadcast(Message(REQUEST_SHIFT_OF_CONTROL,
									(uint8_t)REQUEST_TAKE_CONTROL));

		//Testing globals:
		Globals::angle_control_p.set(0.9);


		vTaskDelay(300);
	}
}


/********************************
 *    PRIVATE FUNCTIONS          *
 ********************************/
void Communication::handle_message(Message& msg){
int i = 0;

	switch(msg.message_type){
		case SHIFT_OF_CONTROL_ACK:
			i++;
			i++;
			break;
		default:
			break;
	}

}

void Communication::handle_internal_message(void)
{

}

void Communication::request_control(uint8_t Request)
{

}


/********************************
 *    COMMUNICATION HANDLERS    *
 ********************************/
void Communication::handle_incomming_communications(void)
{

}

void Communication::handle_outgoing_communications(void)
{

}


/****************************
 *    OUTGOING HANDLERS      *
 ****************************/
void Communication::transmit(void)
{

}

void Communication::transmit_heartbeat(void)
{

}

void Communication::transmit_system_status(void)
{

}

void Communication::transmit_attitude_quaternion(void)
{

}

void Communication::transmit_params_list(void)
{

}

void Communication::transmit_param(uint16_t ID)
{

}

void Communication::transmit_control_ack(uint8_t request, uint8_t ack)
{

}

void Communication::transmit_position(void)
{

}

/****************************
 *    INCOMMING HANDLERS     *
 ****************************/
void Communication::handle_set_mode(void)
{

}

void Communication::handle_param_request_list(void)
{

}

void Communication::handle_param_set(void)
{

}

void Communication::handle_set_roll_pitch_yaw_thrust(void)
{

}

void Communication::handle_change_operator_control(void)
{

}

void Communication::handle_command_long(void)
{

}

void Communication::handle_manual_control(void)
{

}
