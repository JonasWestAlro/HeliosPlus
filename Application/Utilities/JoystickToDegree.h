#ifndef JOYSTICKTODEGREE_H
#define JOYSTICKTODEGREE_H
#include "Framework.h"

#define JOYSTICK_MAX	1000.0
#define JOYSTICK_MIN	-1000.0
#define ANGLE_MAX		25.0
#define ANGLE_MIN		-25.0
#define THROTTLE_MAX	750.0
#define THROTTLE_MIN	0.0

#define JOYSTICK_RANGE  (JOYSTICK_MAX-JOYSTICK_MIN)
#define ANGLE_RANGE		(ANGLE_MAX-ANGLE_MIN)
#define THROTTLE_RANGE  (THROTTLE_MAX-THROTTLE_MIN)
float Joystick_To_Degree(int16_t);
float Joystick_To_Throttle(int16_t);


#endif
