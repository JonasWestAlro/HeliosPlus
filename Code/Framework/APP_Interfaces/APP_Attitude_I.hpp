#pragma once
#include "APP_Interface.hpp"

struct APP_Attitude_I : APP_Interface<APP_Attitude_I>{
	using APP_Interface::APP_Interface;

	float pitch;
	float roll;
	float yaw;
	float pitch_velocity;
	float roll_velocity;
	float yaw_velocity;

	void reset(){
		pitch = 0;
		roll = 0;
		yaw = 0;
		pitch_velocity = 0;
		roll_velocity = 0;
		yaw_velocity = 0;
	}
};
