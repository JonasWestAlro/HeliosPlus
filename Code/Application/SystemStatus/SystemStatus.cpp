#include "SystemStatus.hpp"


SystemStatus::SystemStatus(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
						   Communication* communication, ControlInput* controlinput, FlightControl* flightcontrol,
						   FlightDynamics* flightdynamics, FlightNavigation* flightnavigation)
: ApplicationModule(name, stackSize, priority, eeprom_size),
  communication_module(communication),
  controlinput_module(controlinput),
  flightcontrol_module(flightcontrol),
  flightdynamics_module(flightdynamics),
  flightnavigation_module(flightnavigation)
{
	// Initializer code goes here
	messenger.subscribe(SENSOR_REPORT_STATUS);
	messenger.subscribe(MOTOR_REPORT_STATUS);
	messenger.subscribe(CONTROLINPUT_REPORT_STATUS);
	messenger.subscribe(ARM_REQUEST);
	messenger.subscribe(UNARM_REQUEST);
	messenger.subscribe(REQUEST_SHIFT_OF_CONTROL);


	system_status_socket.armed 		  = false;
	system_status_socket.system_error = false;

	set_frequency(100);
}

void SystemStatus::task(void){
	//Update the Battery Voltage:
	update_battery();

	//Update the state, this doesn't care about timing:
	update_status();

	if(arm_in_progress) check_arm();

	system_status_socket.publish();

	update_leds();
}

void SystemStatus::handle_message(Message& msg){
	switch(msg.type){
		case SENSOR_REPORT_STATUS:
			handle_sensor_report(msg);
			break;

		case MOTOR_REPORT_STATUS:
			handle_motor_report(msg);
			break;

		case CONTROLINPUT_REPORT_STATUS:
			handle_control_input_report(msg);
			break;

		case ARM_REQUEST:
			start_arm();
			break;

		case UNARM_REQUEST:
			system_status_socket.armed = false;
			system_status_socket.mavlink_state = MAV_STATE_STANDBY;
			break;

		case REQUEST_SHIFT_OF_CONTROL:
			handle_shift_of_control(msg);
			break;
		default:
			break;
	}

	return;
}

/*
 * This function implements a state machine which should update
 * the system states. The system states are:
 *
 *  MAV_STATE_UNINIT		Uninitialized system, state is unknown.
 *	MAV_STATE_BOOT			System is booting up.
 *	MAV_STATE_CALIBRATING	System is calibrating and not flight-ready.
 *	MAV_STATE_STANDBY		System is grounded and on standby. It can be launched any time.
 *	MAV_STATE_ACTIVE		System is active and might be already airborne. Motors are engaged.
 *	MAV_STATE_CRITICAL		System is in a non-normal flight mode. It can however still navigate.
 *	MAV_STATE_EMERGENCY		System is in a non-normal flight mode. It lost control over parts or over the whole airframe. It is in mayday and going down.
 *	MAV_STATE_POWEROFF		System just initialized its power-down sequence, will shut down now.
 *
*/
void SystemStatus::update_status(void){
	static uint32_t last_request = 0;

	if(Time.get_time_since_ms(last_request) > 300){
		request_all_to_report();
	}

	//First check if any flight-essentials are NOT OK!
	//Check Conditions:
	switch(conditions_controller.get_status()){
		case STATUS_CALIBRATING:
			system_status_socket.mavlink_state = MAV_STATE_CALIBRATING;
			break;
		case STATUS_NOTOK:
			system_status_socket.mavlink_state = MAV_STATE_CRITICAL;
			break;
		case STATUS_OK:
			if(system_status_socket.armed)
				system_status_socket.mavlink_state = MAV_STATE_ACTIVE;
			else
				system_status_socket.mavlink_state = MAV_STATE_STANDBY;
		default:
			break;
	};
}


void SystemStatus::update_battery(void){
	//TODO-JWA: Implement analog reading
	system_status_socket.voltage_battery = 12300;
}

void SystemStatus::request_all_to_report(void){
	messenger.broadcast(REQUEST_SENSORS_REPORT);
	messenger.broadcast(REQUEST_MOTORS_REPORT);
	messenger.broadcast(REQUEST_CONTROLINPUTS_REPORT);
}

void SystemStatus::start_arm(void){
	request_all_to_report();
	arm_timestamp = Time.get_timestamp();
	arm_in_progress = true;
}

void SystemStatus::check_arm(void){
	//Check for timeout:
	if(Time.get_time_since_ms(arm_timestamp) > 500){
		arm_in_progress = false;
		return;
	}

	//Check Conditions:
	if(conditions_controller.get_status() == STATUS_OK){
		//ALL IS GOOD, READY TO ARM!
		system_status_socket.mavlink_state = MAV_STATE_ACTIVE;
		system_status_socket.armed = true;
		arm_in_progress = false;
	}
}

void SystemStatus::handle_sensor_report(Message& msg){

	uint32_t mavlink_sensors = msg.get_integer32();
	STATUS status = (STATUS)msg.get_enum();

	//Update the conditions table:
	if(mavlink_sensors & MAV_SYS_STATUS_SENSOR_3D_GYRO)
		conditions_controller.update_condition(COND_GYROSCOPE, status);
	if(mavlink_sensors & MAV_SYS_STATUS_SENSOR_3D_ACCEL)
		conditions_controller.update_condition(COND_ACCELEROMETER, status);
	if(mavlink_sensors & MAV_SYS_STATUS_SENSOR_3D_MAG)
		conditions_controller.update_condition(COND_MAGNETOMETER, status);
}

void SystemStatus::handle_motor_report(Message& msg){
	//Update the conditions table:
	conditions_controller.update_condition(COND_MOTORS, (STATUS)msg.get_enum());
}

void SystemStatus::handle_control_input_report(Message& msg){

	APP_InterfaceBase* pipe_of_sender = static_cast<APP_InterfaceBase*>(msg.get_pointer());

	//Check that this is the current input module:
	if(flightcontrol_module->control_socket.get_pipe() == pipe_of_sender){
		conditions_controller.update_condition(COND_CONTROLINPUT, (STATUS)msg.get_enum());
	}
}

void SystemStatus::handle_shift_of_control(Message& msg){
	APP_InterfaceBase* pipe_of_sender = static_cast<APP_InterfaceBase*>(msg.get_pointer());

	if(msg.get_enum() == REQUEST_TAKE_CONTROL){
		flightcontrol_module->control_socket.set_pipe(pipe_of_sender);
	}
}

void SystemStatus::update_leds(void){
	static uint32_t timestamp = 0;
	static uint8_t 	blink_state = 0;

	if(system_status_socket.armed){
		leds->set_LED(STATUS_LED_GREEN, LED_ON);
		leds->set_LED(STATUS_LED_RED, LED_ON);
		return;
	}

	if(system_status_socket.mavlink_state == MAV_STATE_STANDBY){
		//In standby we blink the green LED!
		if(Time.get_time_since_ms(timestamp) > 300){
			if(blink_state){
				leds->set_LED(STATUS_LED_GREEN, LED_ON);
				blink_state = 0;
			}else{
				leds->set_LED(STATUS_LED_GREEN, LED_OFF);
				blink_state = 1;
			}
			timestamp = Time.get_timestamp();
		}

		leds->set_LED(STATUS_LED_RED, LED_OFF);
		return;
	}

	if(system_status_socket.mavlink_state == MAV_STATE_CALIBRATING){
		//In standby we blink the red LED!
		if(Time.get_time_since_ms(timestamp) > 300){
			if(blink_state){
				leds->set_LED(STATUS_LED_RED, LED_ON);
				blink_state = 0;
			}else{
				leds->set_LED(STATUS_LED_RED, LED_OFF);
				blink_state = 1;
			}
			timestamp =  Time.get_timestamp();
		}

		leds->set_LED(STATUS_LED_GREEN, LED_OFF);
		return;
	}

	//If we end up here we're either in emergency or critical!!
	leds->set_LED(STATUS_LED_RED,   LED_ON);
	leds->set_LED(STATUS_LED_GREEN, LED_OFF);
}
