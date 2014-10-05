#pragma once
#include "Framework.hpp"

struct MotorSetup{
	uint8_t motor_number_1; // = motor_output_x
	uint8_t motor_number_2; // = motor_output_x
	uint8_t motor_number_3; // = motor_output_x
	uint8_t motor_number_4; // = motor_output_x
	uint8_t motor_number_5; // = motor_output_x
	uint8_t motor_number_6; // = motor_output_x
	uint8_t motor_number_7; // = motor_output_x
	uint8_t motor_number_8; // = motor_output_x
};

class MotorDistributionController{
public:
	virtual void update(float pitch_command,
					   float roll_command,
					   float yaw_command,
					   uint16_t throttle) = 0;

	uint16_t get_motor_output(uint8_t i){
		return motor_outputs[i];
	}

	void set_motor_setup(MotorSetup new_setup){
		motor_setup = new_setup;
	}

	uint16_t motor_outputs[8];
	MotorSetup motor_setup = {0,1,2,3,4,5,6,7};

private:

};
