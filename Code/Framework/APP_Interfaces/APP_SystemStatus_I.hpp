#pragma once
#include "Framework.hpp"

struct APP_SystemStatus_I : APP_Interface<APP_SystemStatus_I>{
	using APP_Interface::APP_Interface;

	bool 	 armed;
	bool 	 system_error;
	uint8_t  system_error_code;
	uint16_t voltage_battery;
};
