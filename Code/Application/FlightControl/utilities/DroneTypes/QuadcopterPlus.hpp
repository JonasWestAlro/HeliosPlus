#pragma once
#include "Framework.hpp"
#include "MotorDistributionController.hpp"

class QuadcopterPlus : MotorDistributionController{
public:
	virtual void update(float pitch_command, float roll_command, float yaw_command, uint16_t throttle){

		//Compute new motor outputs: local_control.Throttle
		motor_outputs[motor_setup.motor_number_1] = throttle - pitch_command/2.0 - yaw_command;
		motor_outputs[motor_setup.motor_number_2] = throttle + roll_command/2.0 + yaw_command;
		motor_outputs[motor_setup.motor_number_3] = throttle + pitch_command/2.0 - yaw_command;
		motor_outputs[motor_setup.motor_number_4] = throttle - roll_command/2.0 + yaw_command;

		//Constrain all outputs:
		for(uint8_t i=0; i<8; i++){
			if(motor_outputs[i] > 1000.0) motor_outputs[i] = 1000;
			if(motor_outputs[i] < 0) motor_outputs[i] = 0;
		}
	}
};
