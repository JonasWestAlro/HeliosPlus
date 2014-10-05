#pragma once
#include "Framework.hpp"
#include "Quaternion.hpp"

struct APP_Quaternion_I : APP_Interface<APP_Attitude_I>{
	using APP_Interface::APP_Interface;

	float w;
	float x;
	float y;
	float z;

	float pitch_velocity;
	float roll_velocity;
	float yaw_velocity;

    void operator=(const Quaternion &q )
    {
       w = q.w;
       x = q.x;
       y = q.y;
       z = q.z;
    }
};
