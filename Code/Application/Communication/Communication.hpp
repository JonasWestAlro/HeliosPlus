#pragma once

#include "GenericUART.hpp"
#include "Framework.hpp"


class Communication : public ApplicationModule {
	public:
		Communication(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);
		APP_Attitude_I attitude;

	protected:
		void task(void);
		void handle_message(Message& msg);
	private:
		// Functions
		void handle_internal_message(void);
		void request_control(uint8_t);

		//MAVLink Handlers:
		void handle_incomming_communications(void);
		void handle_outgoing_communications(void);

		//OUTGOING HANDLERS:
		void transmit(void);

		void transmit_system_status(void);
		void transmit_heartbeat(void);
		void transmit_attitude_quaternion(void);
		void transmit_params_list(void);
		void transmit_param(uint16_t);
		void transmit_control_ack(uint8_t Request, uint8_t Ack);
		void transmit_position(void);

		//INCOMMING HANDLERS:
		void handle_set_mode(void);
		void handle_param_request_list(void);
		void handle_param_set(void);
		void handle_set_roll_pitch_yaw_thrust(void);
		void handle_change_operator_control(void);
		void handle_command_long(void);
		void handle_manual_control(void);
};
