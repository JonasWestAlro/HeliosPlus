#pragma once
#include "Global.hpp"


namespace Globals{

extern Global<float> angle_control_p;
extern Global<float> angle_control_i;
extern Global<float> angle_control_d;

extern Global<float> rate_control_p;
extern Global<float> rate_control_i;
extern Global<float> rate_control_d;

extern Global<float> yaw_control_p;
extern Global<float> yaw_control_i;
extern Global<float> yaw_control_d;

extern Global<float> throttle_control_alt_p;
extern Global<float> throttle_control_alt_i;
extern Global<float> throttle_control_alt_d;
extern Global<float> throttle_control_alt_imax;
extern Global<float> throttle_control_alt_leash;

extern Global<float> throttle_control_vel_p;
extern Global<float> throttle_control_vel_i;
extern Global<float> throttle_control_vel_d;


	/*Define more global types for
	 * your application here*/
}




