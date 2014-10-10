#pragma once

#include "Framework.hpp"
#include "Communication.hpp"
#include "ControlInput.hpp"
#include "FlightControl.hpp"
#include "FlightDynamics.hpp"
#include "FlightNavigation.hpp"

#include "ConditionsController.hpp"

class SystemStatus : public ApplicationModule{
	public:
		// We need to have eeprom last if we want default size on it
		SystemStatus(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
					 Communication* communication, ControlInput* controlinput, FlightControl* flightcontrol,
					 FlightDynamics* flightdynamics, FlightNavigation* flightnavigation);

		void set_leds(HAL_HeliosLED_I* leds_){
			leds = leds_;
		}

		APP_SystemStatus_I system_status_socket;
	protected:
		void task(void);
		void handle_message(Message& msg);

	private:
		//Drivers:
		HAL_HeliosLED_I* leds;

		//References to all the other modules:
		Communication* 		communication_module;
		ControlInput*  		controlinput_module;
		FlightControl* 		flightcontrol_module;
		FlightDynamics* 	flightdynamics_module;
		FlightNavigation* 	flightnavigation_module;

		ConditionsController conditions_controller;

		bool arm_in_progress = false;
		uint32_t arm_timestamp = 0;

		void update_status(void);
		void update_battery(void);

		void request_all_to_report(void);
		void start_arm(void);
		void check_arm(void);

		void handle_sensor_report(Message& msg);
		void handle_motor_report(Message& msg);
		void handle_control_input_report(Message& msg);
		void handle_shift_of_control(Message& msg);

		void update_leds(void);

};

