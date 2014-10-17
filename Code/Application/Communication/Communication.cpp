#include "Communication.hpp"

#include "Globals.hpp"


Communication::Communication(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{

	set_frequency(50);
}

void Communication::task(void)
{
	//This is done in ApplicationModule
	//Check for internal messages:
	/*while(messenger.try_receive(&local_rx_message)){
		handle_internal_message();
	}*/


	//Handle out going communications:
	handle_outgoing_communications();

	//Check for incomming communications:
	handle_incomming_communications();





	//Testing interface:
	attitude.pitch = 10;
	attitude.receive();

	//Testing message broadcast:
	//messenger.broadcast(CALIBRATE_GYROSCOPE);


	//messenger.broadcast(Message(CALIBRATE_ACCELEROMETER));


	messenger.broadcast(Message(REQUEST_SHIFT_OF_CONTROL,
								(uint8_t)REQUEST_TAKE_CONTROL));

	//TODO-JWA: This status variable should show if this module can generate
	//			a valid input at the moment.
	status = STATUS_OK;

}

void Communication::handle_message(Message& msg){
	Message response;
	switch(msg.type){
		case REQUEST_CONTROLINPUTS_REPORT:
			response.type = CONTROLINPUT_REPORT_STATUS;
			response.set_enum(status);
			messenger.send_to(msg.sender, &response);
			break;
		case SHIFT_OF_CONTROL_REPORT:
			if(msg.get_enum() == YOU_HAVE_CONTROL){
				in_control = true;
			}else{
				in_control = false;
			}
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
	//Heardbeat and system status is transmitted every half a second.
	if(Time.get_time_since_ms(heartbeat_timestamp) > 500){
		transmit_heartbeat();
		//_transmit_system_status();
		heartbeat_timestamp = Time.get_timestamp();
	}

	//Attitude every 50ms
	if(Time.get_time_since_ms(attitude_timestamp) > 200){
		transmit_attitude_quaternion();
		attitude_timestamp = Time.get_timestamp();
	}

	//if(Time.get_time_since_ms(position_timestamp) > 20){
		transmit_position();
		position_timestamp = Time.get_timestamp();
	//}

	//TODO-JWA Transmit Data-Streams
}


/****************************
 *    OUTGOING HANDLERS      *
 ****************************/
void Communication::transmit(void)
{
	uint8_t message_length = mavlink_msg_to_send_buffer(mav_tx_buffer, &mav_tx_message);
	communication->put((const char*)mav_tx_buffer, message_length);
}

void Communication::transmit_heartbeat(void)
{
	system_mode.receive();

	mavlink_msg_heartbeat_pack(mavlink_system.sysid,
							   mavlink_system.compid,
							   &mav_tx_message,
							   MAV_TYPE_QUADROTOR,
							   0,
							   system_mode.mode,
							   0,
							   system_mode.state);

	this->transmit();
}


void Communication::transmit_system_status(void)
{
	mavlink_sys_status_t local_status;
	system_status.receive();

	memcpy(&local_status, &system_status, sizeof(local_status));

	mavlink_msg_sys_status_encode(mavlink_system.sysid,
							      mavlink_system.compid,
							      &mav_tx_message,
							      &local_status);

	this->transmit();
}


void Communication::transmit_attitude_quaternion(void)
{
	attitude.receive();
	quaternion.receive();

	mavlink_msg_attitude_quaternion_pack(mavlink_system.sysid,
		      	  	  	  	  	  	     mavlink_system.compid,
		      	  	  	  	  	  	     &mav_tx_message,
		      	  	  	  	  	  	     Time.get_timestamp(),
		      	  	  	  	  	  	     quaternion.w,
		      	  	  	  	  	  	     quaternion.x,
		      	  	  	  	  			 quaternion.y,
		      	  	  	  	  	  	  	 quaternion.z,
		      	  	  	  	  	  	     attitude.roll_velocity,
		      	  	  	  	  	  	     attitude.pitch_velocity,
		      	  	  	  	  	  	     attitude.yaw_velocity);

	this->transmit();
}


void Communication::transmit_params_list(void)
{
	uint8_t i;
	for (i = 0; i < Globals::get_no_globals(); ++i) {
		this->transmit_param(i);
	}
}


void Communication::transmit_param(uint16_t ID)
{
	const char* param_id = Globals::angle_control_d.get_id();
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
