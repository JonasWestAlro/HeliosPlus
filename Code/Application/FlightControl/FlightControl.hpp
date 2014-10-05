#pragma once

#include "Framework.hpp"
#include "AttitudeController.hpp"
#include "ThrottleController.hpp"

#include "MotorDistributionController.hpp"
#include "Quadcopter.hpp"

class FlightControl : public ApplicationModule{
	public:
		FlightControl(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);
		APP_Attitude_I attitude;

	protected:
		void task(void);
		void handle_message(Message& msg);

		APP_SystemStatus_I system_status_socket;
		APP_Control_I  control_socket;
		APP_Attitude_I attitude_socket;
		APP_Navigation_I navigation_socket;

	private:
		HAL_Motor_I* motors;

		AttitudeController attitude_controller;
		ThrottleController throttle_controller;

		Quadcopter quadcopter;
		MotorDistributionController* distribution_controller;


		float throttle = 0;

		void check_armed();
		void check_control_mode(){}

};

