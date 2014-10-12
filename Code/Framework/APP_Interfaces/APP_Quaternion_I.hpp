#pragma once
#include "APP_Interface.hpp"
#include "Quaternion.hpp"

struct APP_Quaternion_I : APP_Interface<APP_Quaternion_I>{
	using APP_Interface::APP_Interface;

	float w;
	float x;
	float y;
	float z;

    void operator=(const Quaternion &q )
    {
       w = q.w;
       x = q.x;
       y = q.y;
       z = q.z;
    }
};
