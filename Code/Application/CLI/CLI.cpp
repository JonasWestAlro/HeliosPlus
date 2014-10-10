#include "CLI.hpp"

#define BACKSPACE 8
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


		switch(c){
			case CARRIAGE_RETURN:
				Debug.put("\n\r");

				process_buffer();

				last_buffer_index = buffer_index;
				buffer_index = 0;
				Debug.put("\n\n\rFlightController>");
				Debug.transmit();
				break;

			case BACKSPACE:
				if(buffer_index > 0){
					Debug.put_and_transmit("\b \b");
					buffer_index--;
				}
				break;

			case '1':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_duration_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
				}
				break;
			case '2':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_frequency_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
				}
				break;
			case '3':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_stack_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
				}
				break;
			case '4':
				if(buffer_index == 0){
					Debug.put("\n\r");
					handle_print_run_time_stats();
					Debug.put("\n\n\rFlightController>");
					Debug.transmit();
				}
				break;

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

}

void CLI::process_buffer(){
	uint8_t i;
	bool handled = false;

	for(i=0; i<NO_CLI_COMMANDS; i++){
		if(compare_buffer_and_cli_command(i)){
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

bool CLI::compare_buffer_and_cli_command(uint8_t cli_index){
	compare_index = 0;
	uint8_t c = 0;

	while(1){
		c = cli_table[cli_index].command_string[compare_index];

		//If we have reached the end of the command string, we have a match!!
		if(c == '\0'){
			//MATCH:
			return true;
		}

		//If the characters is NOT equal we'll return false:
		if(c != input_buffer[compare_index]){
			return false;
		}

		if(buffer_index == compare_index) return false;

		compare_index++;
	}

}

bool CLI::compare_rest_of_buffer_to(char* compare_string){
	uint8_t i = 0;

	//Check if we have reached to end of input buffer:
	if(compare_index == buffer_index) return false;

	while(1){
		//Check if we have reached the end of the compare string:
		if(compare_string[i] == '\0'){
			return true;
		}

		//Check if we have reached the end of the input buffer:
		if(input_buffer[compare_index] == '\0'){
			return false;
		}

		//Check if the next character match:
		if(compare_string[i] == input_buffer[compare_index]){
			i++;
			compare_index++;
		}else{
			return false;
		}
	}
}


void CLI::handle_stream(void){
	//This is for the space:
	compare_index++;

	if(compare_rest_of_buffer_to("flightdynamics")){
		Debug.put("Streaming FlightDynamics..");
		messenger.send_to(flightdynamics->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = flightdynamics;
		streaming = true;
		return;
	}

	if(compare_rest_of_buffer_to("controlinput")){
		Debug.put("Streaming ControlInput..");
		messenger.send_to(controlinput->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = controlinput;
		streaming = true;
		return;
	}

	if(compare_rest_of_buffer_to("flightcontrol")){
		Debug.put("Streaming FlightControl..");
		messenger.send_to(flightcontrol->get_messenger(), Message(START_DEBUG_STREAM));
		streaming_module = flightcontrol;
		streaming = true;
		return;
	}

	Debug.put("Coulnd't recognize argument?");
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


void CLI::handler2(void){
	Debug.put("HANDLER 2.. ");
}
void handler3(void){
	Debug.put("HANDLER 3.. ");
}
