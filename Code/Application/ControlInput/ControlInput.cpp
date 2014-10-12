#include "Application/ControlInput/ControlInput.hpp"


ControlInput::ControlInput(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	messenger.subscribe(REQUEST_CONTROLINPUTS_REPORT);
	messenger.subscribe(CALIBRATE_CONTROLINPUT);

	set_frequency(50);
}

void ControlInput::task(void){

	control_receiver->execute();
	update_status();

	//If the control receiver is OK we'll get data and publish:
	if(status == STATUS_OK){

		//Get Raw controller values:
		//HACK-ALERT: ROLL AND YAW ARE ONLY INVERTED DUE TO THE DX6 transmitter!
		control_receiver->get_all_channels(raw_values);
		raw_values.roll *= -1;
		raw_values.yaw  *= -1;

		//convert to attitude:
		control_socket.pitch 	= convert_joystick_to_degree(raw_values.pitch);
		control_socket.roll 	= convert_joystick_to_degree(raw_values.roll);
		control_socket.yaw 		= convert_joystick_to_degree(raw_values.yaw);
		control_socket.throttle = convert_joystick_to_throttle(raw_values.throttle);


		//Update switch and clicker:
		aux_states[0]  =  raw_values.aux1;
		aux_states[1]  =  raw_values.aux2;

		if(aux_states[0] != last_aux_states[0])   handle_clicker(aux_states[0]);
		if(aux_states[1] != last_aux_states[1])  handle_switch(aux_states[1]);

		last_aux_states[0] = aux_states[0];
		last_aux_states[1] = aux_states[1];

		//If we are in fixed altitude mode, we'll integrate the throttle stick
		//to get a new altitude.
		if(control_socket.control_mode == CONTROLMODE_FIXED_ALTITUDE){
			process_altitude_setpoint();
		}

		control_socket.altitude = contrain(control_socket.altitude, 0, 300);
	}else{
		//THE CONTROL RECEIVER ISN'T OK!!
		control_socket.reset();
	}

	control_socket.publish();

	check_calibration();
	check_arm();
	if(debugging_stream) handle_debug_stream();
}

/****************************************************************
 * 						PRIVATE FUNCTIONS						*
 ****************************************************************/
void ControlInput::handle_message(Message& msg){
	Message respons;

	switch(msg.type){
		case REQUEST_CONTROLINPUTS_REPORT:
			respons.type = CONTROLINPUT_REPORT_STATUS;
			respons.set_enum(status);
			respons.set_byte(0, MANUAL_CONTROL_INPUT);
			respons.set_pointer(static_cast<void*>(control_socket.get_pipe()));
			messenger.send_to(msg.sender, &respons);
			break;

		case SHIFT_OF_CONTROL_ACK:
			if((msg.get_byte(0) == REQUEST_TAKE_CONTROL) &&
				msg.get_byte(1) == ACK 	){
				in_control = true;
			}else{
				in_control = false;
			}
			break;

		case CALIBRATE_CONTROLINPUT:
			control_receiver->start_calibration();
			calibrate_timestamp = Time.get_timestamp();
			calibrating = true;
			break;
		case START_DEBUG_STREAM:
			debugging_stream = true;
			break;

		case STOP_DEBUG_STREAM:
			debugging_stream = false;
			break;
		default:
			break;
	}
}

void ControlInput::update_status(void){
	if(calibrating){
		if(status != STATUS_CALIBRATING){
			status = STATUS_CALIBRATING;
			report_status();
		}
		return;
	}

	//Check that driver status is OK:
	if(control_receiver->get_status() == STATUS_OK){
		if(status != STATUS_NOTOK){
			status = STATUS_OK;
			report_status();
		}
	}else{
		//Status is not OK!..
		//Check for transistion:
		if(status != STATUS_OK){
			status = STATUS_NOTOK;
			report_status();
		}
	};
}

void ControlInput::process_altitude_setpoint(){
	if(Time.get_time_since_ms(altitude_integral_timestamp) > 50){
		if(raw_values.throttle > 350)
			control_socket.altitude += 1.0;
		if(raw_values.throttle < -350)
			control_socket.altitude -= 1.0;
		altitude_integral_timestamp = Time.get_timestamp();
	}
}

void ControlInput::handle_switch(float state)
{

}

void ControlInput::handle_clicker(float state)
{

}

void ControlInput::check_arm(void){
	static uint32_t timestamp = 0;
	static bool  switch_in_progress = false;

	//Check conditions:
	if(control_socket.throttle < 10 && raw_values.yaw < -930){
		if(!switch_in_progress){
			switch_in_progress = true;
			timestamp = Time.get_timestamp();
		}else{
			//A switch is ongoing. First check if it is done!
			if(Time.get_time_since_ms(timestamp) > 1000){
				switch_arm_state();
				switch_in_progress = false;
			}
		}
	}else{
		switch_in_progress = false;
	}
}

void ControlInput::switch_arm_state(void){
	system_status_socket.receive();

	if(!system_status_socket.armed){
		messenger.broadcast(ARM_REQUEST);
	}else{
		messenger.broadcast(UNARM_REQUEST);
	}
}

void ControlInput::request_control(uint8_t Request){
	messenger.broadcast(Message(REQUEST_SHIFT_OF_CONTROL, (uint8_t)REQUEST_TAKE_CONTROL));
}


void ControlInput::report_status(void){
	messenger.broadcast(Message(CONTROLINPUT_REPORT_STATUS, (uint8_t)status));
}


void ControlInput::check_calibration(){
	if(calibrating){
		if(Time.get_time_since_sec(calibrate_timestamp) > 10){
			control_receiver->stop_calibration();
			calibrating = false;
		}
	}
}

void ControlInput::handle_debug_stream(){
	static uint32_t timestamp = 0;

	if(Time.get_time_since_ms(timestamp)>50){
		Debug.send_and_transmit_floats({
			control_socket.throttle,
			control_socket.yaw,
			control_socket.pitch,
			control_socket.roll
		});
		timestamp = Time.get_timestamp();
	}
}

#define JOYSTICK_MAX	1000.0
#define JOYSTICK_MIN	-1000.0
#define ANGLE_MAX		25.0
#define ANGLE_MIN		-25.0
#define THROTTLE_MAX	750.0
#define THROTTLE_MIN	0.0
#define JOYSTICK_RANGE  (JOYSTICK_MAX-JOYSTICK_MIN)
#define ANGLE_RANGE		(ANGLE_MAX-ANGLE_MIN)
#define THROTTLE_RANGE  (THROTTLE_MAX-THROTTLE_MIN)

float ControlInput::convert_joystick_to_degree(int16_t value){
	if(value>JOYSTICK_MAX || value<JOYSTICK_MIN )
		return 0;

	return (ANGLE_RANGE/JOYSTICK_RANGE)*value;
}

float ControlInput::convert_joystick_to_throttle(int16_t value){
	if(value>JOYSTICK_MAX || value<JOYSTICK_MIN )
			return 0;

	return (float)(value+JOYSTICK_RANGE/2.0)*(THROTTLE_RANGE/JOYSTICK_RANGE);
}


float ControlInput::contrain(float variable, float min, float max){
	if(variable > max) return max;
	if(variable < min) return min;

	return variable;
}
