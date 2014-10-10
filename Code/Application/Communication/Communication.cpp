#include "Communication.hpp"

#include "Globals.hpp"


Communication::Communication(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{

	set_frequency(50);
}

void Communication::task(void){

	//Testing interface:
	attitude.pitch = 10;
	attitude.receive();

	//Testing message breoadcast:
	messenger.broadcast(CALIBRATE_GYROSCOPE);


	messenger.broadcast(Message(CALIBRATE_ACCELEROMETER));


	messenger.broadcast(Message(REQUEST_SHIFT_OF_CONTROL,
								(uint8_t)REQUEST_TAKE_CONTROL));

	//Testing globals:
	Globals::angle_control_p.set(0.9);

}


/********************************
 *    PRIVATE FUNCTIONS          *
 ********************************/
void Communication::handle_message(Message& msg){
int i = 0;

	switch(msg.type){
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
