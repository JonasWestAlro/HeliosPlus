#include "JoystickToDegree.h"

/*
 * This utility function calculates
 * an angle based on a joystick
 */

float Joystick_To_Degree(int16_t value){
	if(value>JOYSTICK_MAX || value<JOYSTICK_MIN )
		return 0;

	return (ANGLE_RANGE/JOYSTICK_RANGE)*value;

}

float Joystick_To_Throttle(int16_t value){

	if(value>JOYSTICK_MAX || value<JOYSTICK_MIN )
			return 0;

	return (float)(value+JOYSTICK_RANGE/2.0)*(THROTTLE_RANGE/JOYSTICK_RANGE);


}



