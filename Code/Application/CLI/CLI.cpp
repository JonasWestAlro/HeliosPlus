#include "CLI.hpp"

#define BACKSPACE 		8
#define TABULATOR		9
#define CARRIAGE_RETURN 13

CLI::CLI(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
		ApplicationModule* communications_,
		ApplicationModule* controlinput_,
		ApplicationModule* flightcontrol_,
		ApplicationModule* flightdynamics_,
		ApplicationModule* flightnavigation_,
		ApplicationModule* systemstatus_)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	communications 		=  communications_;
	controlinput 		=  controlinput_;
	flightcontrol 		=  flightcontrol_;
	flightdynamics 		=  flightdynamics_;
	flightnavigation 	=  flightnavigation_;
	systemstatus 		=  systemstatus_;

	set_frequency(50);
}

void CLI::task(void){
	uint8_t c;
	uint8_t i;

	while(Debug.data_available()){
		Debug.receive(&c);

		if(streaming){
			messenger.send_to(streaming_module->get_messenger(), Message(STOP_DEBUG_STREAM));
			Debug.put("\n\n\rStopping Stream..");
			Debug.put("\n\n\rFlightController>");
			Debug.transmit();
			streaming = false;
			return;
		}

		//Check if we have requested some other module to print something:
		if(waiting_for_external_print){
			//We have requested an other module to print something, so we'll have to wait.
			//Though, we'll check if the timeout has been reached:
			if(Time.get_time_since_ms(external_print_request_timestamp) > 500){
				Debug.put("External module never reacted :( ..");
				Debug.put("\n\n\rFlightController>");
				Debug.transmit();
				waiting_for_external_print = false;
			}
		}

		switch(c){
			case CARRIAGE_RETURN:
				if(buffer_index > 0){
					Debug.put("\n\r");

					process_buffer();

					memcpy(last_input_buffer, input_buffer, buffer_index);
					last_buffer_index = buffer_index;
					buffer_index = 0;
					compare_index = 0;

					if(!waiting_for_external_print){
						Debug.put("\n\n\rFlightController>");
					}
					Debug.transmit();
				}else{
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
				}
				break;

			case BACKSPACE:
				if(buffer_index > 0){
					Debug.put_and_transmit("\b \b");
					buffer_index--;
				}
				break;

			case TABULATOR:
				for (i = 0; i < buffer_index; ++i) {
					Debug.put("\b \b");
				}
				buffer_index = last_buffer_index;
				memcpy(input_buffer, last_input_buffer, last_buffer_index);
				Debug.put((const char*)input_buffer, buffer_index);
				Debug.transmit();
				break;

			case '1':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_duration_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
					break;
				}

			case '2':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_frequency_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
					break;
				}

			case '3':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_stack_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
					break;
				}

			case '4':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_run_time_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
					break;
				}


			default:
				Debug.send_and_transmit(c);
				input_buffer[buffer_index++] = c;
				if(buffer_index == 255){
					handle_buffer_full();
				}
				break;
		}
	}
}

void CLI::handle_message(Message& msg){
	switch(msg.type){
			case CLI_ACK_PRINT:
				waiting_for_external_print = false;
				Debug.put("\n\n\rFlightController>");
				Debug.transmit();
				break;
			default:
				break;
		}
}

void CLI::process_buffer(){
	uint8_t i;
	bool handled = false;

	for(i=0; i<NO_CLI_COMMANDS; i++){
		if(compare_next_word_to(cli_table[i].command_string)){
			//We have a match, call handler:
			cli_table[i].raise(this);
			handled = true;
			break;
		}
	}

	if(!handled){
		Debug.put("Command not found!");
	}
}

bool CLI::compare_next_word_to(const char* compare_command){
	uint8_t offset = compare_index;
	uint8_t compare_command_length = strlen(compare_command);

	// Rest of buffer
	uint8_t len = buffer_index - offset;

	// Do we have a space before '\0' ?
	uint8_t *ptr = (uint8_t*)memchr(&input_buffer[offset], ' ', len);

	// We have a space. Only compare next word
	// This will also guarantee that trailing spaces wont be checked
	if(ptr != 0){
		len = ptr - &input_buffer[offset];
	}

	if(len == compare_command_length &&
	   memcmp(compare_command, &input_buffer[offset], compare_command_length) == 0){
		// Next time we have to compare from this index
		compare_index += compare_command_length + 1;
		return true;
	}else{
		return false;
	}
}

bool CLI::get_next_word_as_number(float& number){
	uint8_t offset = compare_index;

	// Rest of buffer
	uint8_t len = buffer_index - offset;

	// Do we have a space before '\0' ?
	uint8_t* ptr = (uint8_t*)memchr(&input_buffer[offset], ' ', len);

	// We have a space. Only compare next word
	// This will also guarantee that trailing spaces wont be checked
	if(ptr != 0){
		len = ptr - &input_buffer[offset];
	}

	number = atof((char*)(input_buffer+offset));
	compare_index += len;
	return 1;
}

void CLI::handle_help(void){
	Debug.put("****************************************\n\r");
	Debug.put("        HELIOS FLIGHTCONTROLLER CLI     \n\r");
	Debug.put("****************************************\n\r");
	Debug.put("Welcome, the following commands can be used:\n\n\r");
	Debug.put("\t- stream <module> \n\r");
	Debug.put("\t- print <attribute(s)>\n\r");
	Debug.put("\t- arm \n\r");
	Debug.put("\t- unarm\n\r");
}

void CLI::handle_stream(void){
	if(compare_next_word_to("flightdynamics")){
		Debug.put("Streaming FlightDynamics..");
		messenger.send_to(flightdynamics->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = flightdynamics;
		streaming = true;
	}
	else if(compare_next_word_to("controlinput")){
		Debug.put("Streaming ControlInput..");
		messenger.send_to(controlinput->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = controlinput;
		streaming = true;
	}
	else if(compare_next_word_to("flightcontrol")){
		Debug.put("Streaming FlightControl..");
		messenger.send_to(flightcontrol->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = flightcontrol;
		streaming = true;
	}
	else{
		Debug.put("Usage: stream <name>\n\r\n\r");
		Debug.put("Name:\n\r");
		Debug.put("\t- flightdynamics\n\r");
		Debug.put("\t- controlinput\n\r");
		Debug.put("\t- flightcontrol\n\r");
	}
}

void CLI::handle_print(void){
	if(compare_next_word_to("run_time_stats")){
		handle_print_run_time_stats();
	}
	else if(compare_next_word_to("stack_stats")){
		handle_print_stack_stats();
	}
	else if(compare_next_word_to("frequency_stats")){
		handle_print_frequency_stats();
	}
	else if(compare_next_word_to("duration_stats")){
		handle_print_duration_stats();
	}
	else if(compare_next_word_to("arm_conditions")){
		handle_print_arming_conditions();
	}
	else if(compare_next_word_to("parameters")){
		handle_print_parameters();
	}
	else{
		Debug.put("Usage: print <name>\n\r\n\r");
		Debug.put("Name:\n\r");
		Debug.put("\t- run_time_stats\n\r");
		Debug.put("\t- stack_stats\n\r");
		Debug.put("\t- frequency_stats\n\r");
		Debug.put("\t- duration_stats\n\r");
		Debug.put("\t- arm_conditions\n\r");
		Debug.put("\t- parameters\n\r");
	}
}

void CLI::handle_print_run_time_stats(void){
	flightdynamics->getRunTimeStats(run_time_stats);

	Debug.put("****************************************\n\r");
	Debug.put("             RUN TIME STATS             \n\r");
	Debug.put("****************************************\n\r");

	Debug.put(run_time_stats);
}

void CLI::handle_print_stack_stats(void){
	//flightdynamics->getRunTimeStats(run_time_stats);

	Debug.put("****************************************\n\r");
	Debug.put("              STACK STATS               \n\r");
	Debug.put("****************************************\n\r");
	Debug.put("FlightDynamics:.......");
	Debug.send_number(flightdynamics->getFreeStack());
	Debug.put("\n\r");
	Debug.put("ControlInput:.........");
	Debug.send_number(controlinput->getFreeStack());
	Debug.put("\n\r");
	Debug.put("FlightControl:........");
	Debug.send_number(flightcontrol->getFreeStack());
	Debug.put("\n\r");
	Debug.put("FlightNavigation:.....");
	Debug.send_number(flightnavigation->getFreeStack());
	Debug.put("\n\r");
	Debug.put("Communications:.......");
	Debug.send_number(communications->getFreeStack());
	Debug.put("\n\r");
	Debug.put("SystemStatus:.........");
	Debug.send_number(systemstatus->getFreeStack());
	Debug.put("\n\r");
}

void CLI::handle_print_frequency_stats(void){

	Debug.put("****************************************\n\r");
	Debug.put("          FREQUENCY STATS               \n\r");
	Debug.put("****************************************\n\r");
	Debug.put("FlightDynamics:.......");
	Debug.send_number(flightdynamics->get_measured_frequency());
	Debug.put("\n\r");
	Debug.put("ControlInput:.........");
	Debug.send_number(controlinput->get_measured_frequency());
	Debug.put("\n\r");
	Debug.put("FlightControl:........");
	Debug.send_number(flightcontrol->get_measured_frequency());
	Debug.put("\n\r");
	Debug.put("FlightNavigation:.....");
	Debug.send_number(flightnavigation->get_measured_frequency());
	Debug.put("\n\r");
	Debug.put("Communications:.......");
	Debug.send_number(communications->get_measured_frequency());
	Debug.put("\n\r");
	Debug.put("SystemStatus:.........");
	Debug.send_number(systemstatus->get_measured_frequency());
	Debug.put("\n\r");
}

void CLI::handle_print_duration_stats(void){

	Debug.put("****************************************\n\r");
	Debug.put("          DURATION STATS                \n\r");
	Debug.put("****************************************\n\r");
	Debug.put("FlightDynamics:.......");
	Debug.send_number(flightdynamics->get_measured_duration());
	Debug.put("\n\r");
	Debug.put("ControlInput:.........");
	Debug.send_number(controlinput->get_measured_duration());
	Debug.put("\n\r");
	Debug.put("FlightControl:........");
	Debug.send_number(flightcontrol->get_measured_duration());
	Debug.put("\n\r");
	Debug.put("FlightNavigation:.....");
	Debug.send_number(flightnavigation->get_measured_duration());
	Debug.put("\n\r");
	Debug.put("Communications:.......");
	Debug.send_number(communications->get_measured_duration());
	Debug.put("\n\r");
	Debug.put("SystemStatus:.........");
	Debug.send_number(systemstatus->get_measured_duration());
	Debug.put("\n\r");
}

void CLI::handle_print_parameters(void){
	char numberbuffer[10] = {0};

	Debug.put("****************************************\n\r");
	Debug.put("          	  PARAMETERS               \n\r");
	Debug.put("****************************************\n\r");

	GlobalAbstract** globals_table = Globals::get_globals_table();

	for(uint8_t i = 0; i<Globals::get_no_globals(); i++){
		Debug.put(globals_table[i]->get_id());
		Debug.put("\t\t\t");

		globals_table[i]->print(numberbuffer);
		Debug.put(numberbuffer);
		Debug.put("\n\r");
	}
}

void CLI::handle_calibrate(void){
	if(compare_next_word_to("gyroscope")){
		messenger.broadcast(CALIBRATE_GYROSCOPE, CALIBRATION_START);
		handle_calibrate_process("gyroscope");
	}
	else if(compare_next_word_to("accelerometer")){
		messenger.broadcast(CALIBRATE_ACCELEROMETER, CALIBRATION_START);
		handle_calibrate_process("accelerometer");
	}
	else if(compare_next_word_to("magnetometer")){
		messenger.broadcast(CALIBRATE_MAGNETOMETER, CALIBRATION_START);
		handle_calibrate_process("magnetometer");
	}
	else if(compare_next_word_to("controlinput")){
		messenger.broadcast(CALIBRATE_CONTROLINPUT, CALIBRATION_START);
		handle_calibrate_process("controlinput");
	}
	else{
		Debug.put("Usage: calibrate <sensor>\n\r\n\r");
		Debug.put("Sensor:\n\r");
		Debug.put("\t- gyroscope\n\r");
		Debug.put("\t- accelerometer\n\r");
		Debug.put("\t- magnetometer\n\r");
		Debug.put("\t- controlinput\n\r");
	}
}

void CLI::handle_calibrate_process(char* sensor){
	messenger.broadcast(CALIBRATE_GYROSCOPE, CALIBRATION_START);

	Debug.put("Calibrating ");
	Debug.put_and_transmit(sensor);

	//Schedule us out for 0.5 sec:
	schedule_out_ms(500);

	//Check that the system indeed is in calibration mode:
	system_status_socket.receive();

	if(system_status_socket.mavlink_state == MAV_STATE_CALIBRATING){
		//All is good we are actually calibrating!
		//Now we'll wait for it to be done..
		while(1){
			schedule_out_ms(500);
			system_status_socket.receive();
			if(system_status_socket.mavlink_state != MAV_STATE_CALIBRATING){
				Debug.put("\n\rCalibration done!");
				break;
			}else{
				//Still calibrating, we'll show a sign of life:
				Debug.send_and_transmit('.');

				//We'll also check if the user has pushed a button,
				//If he has, we'll exit the calibration..
				if(Debug.data_available()){
					Debug.put("\n\rExited calibration monitor before calibration was done.");
					break;
				}
			}
		}
	}else{
		//It seems that the calibration really actually started..
		Debug.put(" - error, calibration never started :(");
	}

}

void CLI::handle_print_arming_conditions(void){
	messenger.send_to(systemstatus->get_messenger(),
					  Message(CLI_PRINT_REQUEST));

	external_print_request_timestamp = Time.get_timestamp();
	waiting_for_external_print = true;
}

void CLI::handle_arm(void){
	messenger.broadcast(ARM_REQUEST);
	Debug.put("Trying to ARM...");
}

void CLI::handle_unarm(void){
	messenger.broadcast(UNARM_REQUEST);
	Debug.put("UNARMING...");
}

void CLI::handle_set(void){
	if(compare_next_word_to("parameter")){
		handle_set_parameter();
	}
	else{
		Debug.put("Usage: set <attribute type><attribute>\n\r\n\r");
		Debug.put("attribute type:\n\r");
		Debug.put("\t- parameter\n\r");
	}
}

void CLI::handle_set_parameter(void){
	bool success = false;

	//Find out which parameter..
	GlobalAbstract** globals_table = Globals::get_globals_table();

	for(uint8_t i = 0; i<Globals::get_no_globals(); i++){
		if(compare_next_word_to(globals_table[i]->get_id())){
			float new_parameter;
			if(get_next_word_as_number(new_parameter)){
				static_cast<Global<float>*>(globals_table[i])->set(new_parameter);

				Debug.put(globals_table[i]->get_id());
				Debug.put(" is now set to: ");
				Debug.send_number(new_parameter);
				Debug.put("\n\r");
				success = true;
				break;
			}
		}
	}

	if(!success){
		Debug.put("Error.. Not able to find the specified parameter :(");
	}

}
