#pragma once

#include "Framework.hpp"
#include "Debug.hpp"
#include "Globals.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* atof */

#define NO_CLI_COMMANDS 8

class CLI;

struct CLI_Command
{
    CLI_Command(const char* s, void (CLI::*h)(void))
    {
        command_string = s;
        handler = h;
    }

    const char* command_string;
    void (CLI::*handler)(void);

    void raise( CLI* the_cli ) { return (the_cli->*handler)(); }
};

class CLI : public ApplicationModule {
	public:
		CLI(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
			ApplicationModule* communications_,
			ApplicationModule* controlinput_,
			ApplicationModule* flightcontrol_,
			ApplicationModule* flightdynamics_,
			ApplicationModule* flightnavigation_,
			ApplicationModule* systemstatus_);

			APP_SystemStatus_I system_status_socket;
			APP_Control_I 	   control_socket;

	protected:
		void task(void);
		void handle_message(Message& msg);

	private:
		ApplicationModule* flightdynamics;
		ApplicationModule* controlinput;
		ApplicationModule* flightcontrol;
		ApplicationModule* flightnavigation;
		ApplicationModule* communications;
		ApplicationModule* systemstatus;

		uint8_t input_buffer[256] = {0};
		uint8_t last_input_buffer[256]   = {0};
		uint8_t buffer_index = 0;
		uint8_t last_buffer_index = 0;
		uint8_t compare_index = 0;

		uint32_t external_print_request_timestamp = 0;
		bool waiting_for_external_print = false;
		bool streaming = false;
		ApplicationModule* streaming_module;

		char run_time_stats[40*10];

		STATUS status = STATUS_NOTOK;
		bool in_control = false;

		void process_buffer();
		bool compare_next_word_to(const char* compare_command);
		bool get_next_word_as_number(float&);
		uint8_t wait_for_input(char* compare, uint8_t max_lenght);

        CLI_Command cli_table[NO_CLI_COMMANDS] = {
          { "help",		&CLI::handle_help },
          { "stream",	&CLI::handle_stream },
          { "print",	&CLI::handle_print },
          { "calibrate", &CLI::handle_calibrate },
          { "arm",		&CLI::handle_arm },
          { "unarm",	&CLI::handle_unarm },
          { "set",		&CLI::handle_set },
          { "flymode",	&CLI::handle_flymode }
        };
		void handle_buffer_full(){};

		void handle_help(void);
		void handle_stream(void);

		void handle_print(void);
		void handle_print_run_time_stats(void);
		void handle_print_stack_stats(void);
		void handle_print_frequency_stats(void);
		void handle_print_duration_stats(void);
		void handle_print_arming_conditions(void);
		void handle_print_parameters(void);

		void handle_calibrate(void);
		void handle_calibrate_process(char*);

		void handle_arm(void);
		void handle_unarm(void);

		void handle_set(void);
		void handle_set_parameter(void);

		void handle_flymode(void);
		void handle_flymode_program(void);
		void handle_flymode_print_controls(void);

};
