#pragma once
#include "Framework.hpp"

typedef enum{
	CONTROLMODE_MANUAL_THROTTLE = 0,
	CONTROLMODE_FIXED_ALTITUDE,
}ControlMode;

struct APP_Control_I : APP_Interface<APP_Control_I>{
	using APP_Interface::APP_Interface;

	float pitch;
	float roll;
	float yaw;
	float throttle;
	float altitude;
	ControlMode control_mode;

	void reset(){
		pitch 	= 0;
		roll 	= 0;
		yaw 	= 0;
		throttle = 0;
		altitude = 0;
		control_mode = CONTROLMODE_MANUAL_THROTTLE;
	}
};
