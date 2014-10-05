#pragma once
#include "Framework.hpp"

struct APP_Attitude_I : APP_Interface<APP_Attitude_I>{
	using APP_Interface::APP_Interface;

	float pitch;
	float roll;
	float yaw;
	float pitch_velocity;
	float roll_velocity;
	float yaw_velocity;
};
