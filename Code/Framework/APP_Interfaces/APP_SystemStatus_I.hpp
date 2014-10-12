#pragma once
#include "APP_Interface.hpp"
#include "mavlink.h"

struct APP_SystemStatus_I : APP_Interface<APP_SystemStatus_I>{
	using APP_Interface::APP_Interface;

	uint32_t onboard_control_sensors_present; 	//!< Bitmask showing which onboard controllers and sensors are present. Value of 0: not present. Value of 1: present. Indices defined by ENUM MAV_SYS_STATUS_SENSOR
	uint32_t onboard_control_sensors_enabled; 	//!< Bitmask showing which onboard controllers and sensors are enabled:  Value of 0: not enabled. Value of 1: enabled. Indices defined by ENUM MAV_SYS_STATUS_SENSOR
	uint32_t onboard_control_sensors_health; 	//!< Bitmask showing which onboard controllers and sensors are operational or have an error:  Value of 0: not enabled. Value of 1: enabled. Indices defined by ENUM MAV_SYS_STATUS_SENSOR
	uint16_t load; 								//!< Maximum usage in percent of the mainloop time, (0%: 0, 100%: 1000) should be always below 1000
	uint16_t voltage_battery; 					//!< Battery voltage, in millivolts (1 = 1 millivolt)
	int16_t  current_battery; 					//!< Battery current, in 10*milliamperes (1 = 10 milliampere), -1: autopilot does not measure the current
	uint16_t drop_rate_comm; 					//!< Communication drops in percent, (0%: 0, 100%: 10'000), (UART, I2C, SPI, CAN), dropped packets on all links (packets that were corrupted on reception on the MAV)
	uint16_t errors_comm; 						//!< Communication errors (UART, I2C, SPI, CAN), dropped packets on all links (packets that were corrupted on reception on the MAV)
	uint16_t errors_count1; 					//!< Autopilot-specific errors
	uint16_t errors_count2; 					//!< Autopilot-specific errors
	uint16_t errors_count3; 					//!< Autopilot-specific errors
	uint16_t errors_count4; 					//!< Autopilot-specific errors
	int8_t   battery_remaining; 				//!< Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot estimate the remaining battery

	MAV_STATE mavlink_state;
	bool	  armed;
	bool	  system_error;
};
