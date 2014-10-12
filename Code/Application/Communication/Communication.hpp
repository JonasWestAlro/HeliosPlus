#pragma once

#include "mavlink.h"

#include "GenericUART.hpp"
#include "Framework.hpp"


class Communication : public ApplicationModule {
	public:
		Communication(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);

		// Interfaces
		APP_Attitude_I 	   attitude;
		APP_Quaternion_I   quaternion;
		APP_SystemMode_I   system_mode;
		APP_SystemStatus_I system_status;

		void set_driver(HAL_Communication_I* com){communication = com;};

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

		uint32_t heartbeat_timestamp = 0;
		uint32_t attitude_timestamp  = 0;
		uint32_t position_timestamp  = 0;

		uint8_t mav_tx_buffer[MAVLINK_MAX_PACKET_LEN];

		mavlink_message_t mav_rx_message;
		mavlink_message_t mav_tx_message;

		const mavlink_system_t mavlink_system = {
							    20,		// System ID
							    200,	// Component ID
								0,		// Unused
								0,		// Unused
								0,		// Unused
								0};		// Unused

		HAL_Communication_I* communication = 0;
};
