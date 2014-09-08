#pragma once
#include "Global.hpp"


namespace Globals{

Global<float> angle_control_p( 6.7 , "ANGLECONTROL_P\0");
Global<float> angle_control_i( 0.2 , "ANGLECONTROL_I\0");
Global<float> angle_control_d( 0.0 , "ANGLECONTROL_D\0");

Global<float> rate_control_p( 0.8 , "RATECONTROL_P\0");
Global<float> rate_control_i( 0.0 , "RATECONTROL_I\0");
Global<float> rate_control_d( 0.005 , "RATECONTROL_D\0");

Global<float> yaw_control_p( 4.0 , "YAWCONTROL_P\0");
Global<float> yaw_control_i( -2.0 , "YAWCONTROL_I\0");
Global<float> yaw_control_d( 0.0 , "YAWCONTROL_D\0");

Global<float> throttle_control_alt_p( 3.0 , "THRCONT_ALT_P\0");
Global<float> throttle_control_alt_i( -1.0, "THRCONT_ALT_I\0");
Global<float> throttle_control_alt_d( 0.0 , "THRCONT_ALT_D\0");
Global<float> throttle_control_alt_imax( 200 , "THRCONT_IMAX\0");
Global<float> throttle_control_alt_leash( 20 , "THRCONT_LEASH\0");

Global<float> throttle_control_vel_p( 1.5 , "THRCONT_VEL_P\0");
Global<float> throttle_control_vel_i( 0.0 , "THRCONT_VEL_I\0");
Global<float> throttle_control_vel_d( 0.0 , "THRCONT_VEL_D\0");


	/*Define more global types for
	 * your application here*/
}




