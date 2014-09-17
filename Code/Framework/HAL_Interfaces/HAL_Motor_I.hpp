#pragma once

#include "Framework.hpp"

typedef enum{
	MOTOR_1 = 0b1,
	MOTOR_2 = 0b10,
	MOTOR_3 = 0b100,
	MOTOR_4 = 0b1000,
	MOTOR_5 = 0b10000,
	MOTOR_6 = 0b100000,
	MOTOR_7 = 0b1000000,
	MOTOR_8 = 0b10000000,
	MOTOR_ALL = 0xFF
}MOTOR;

class HAL_Motor_I : public HAL_Interface {
	public:
	virtual void set_motor_speed(MOTOR, uint16_t) = 0;
};
