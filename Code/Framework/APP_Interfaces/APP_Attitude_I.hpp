#pragma once
#include "Framework.hpp"

struct APP_Attitude_I : APP_Interface<APP_Attitude_I>{
	using APP_Interface::APP_Interface;

	float Pitch;
	float Roll;
	float Yaw;
	float PitchVelocity;
	float RollVelocity;
	float YawVelocity;
};
