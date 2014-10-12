#pragma once

#include "Framework.hpp"
#include "MavLink.h"
#include "Debug.hpp"

class ControlInput : public ApplicationModule{
	public:
		ControlInput(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);

		void set_control_receiver(HAL_ControlReceiver_I* p){control_receiver = p;}

		APP_Control_I 		control_socket;
		APP_SystemStatus_I 	system_status_socket;

	protected:
		void task(void);
		void handle_message(Message& msg);

	private:
		HAL_ControlReceiver_I* control_receiver;

		bool debugging_stream = false;
		bool in_control = true;
		bool calibrating = false;
		uint32_t calibrate_timestamp;
		STATUS status = STATUS_OK;

		int16_t aux_states[2]	 	= {0};
		int16_t last_aux_states[2] 	= {0};

		ControlReceiverValues raw_values;
		uint32_t altitude_integral_timestamp;

		void handle_internal_message(void);
		void process_altitude_setpoint();
		void handle_switch(float state);
		void handle_clicker(float state);
		void check_arm(void);
		void switch_arm_state(void);
		void request_control(uint8_t Request);

		void check_calibration();

		void update_status(void);
		void report_status(void);

		void handle_debug_stream();


		float convert_joystick_to_degree(int16_t);
		float convert_joystick_to_throttle(int16_t);

		float contrain(float variable, float min, float max);
};
