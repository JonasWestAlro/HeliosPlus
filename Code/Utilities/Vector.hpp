#pragma once
#include "arm_math.h"

#define DEGREE_TO_RAD (2*M_PI)/360.0

class Vector{
public:
	float x;
	float y;
	float z;

	Vector(){};
	Vector(float x_, float y_, float z_){
		x = x_;
		y = y_;
		z = z_;
	}

	void convert_to_frame(Vector& angles){
		float cosX = arm_cos_f32(angles.x*DEGREE_TO_RAD);
		float cosY = arm_cos_f32(angles.y*DEGREE_TO_RAD);
		float cosZ = arm_cos_f32(angles.z*DEGREE_TO_RAD);
		float sinX = arm_sin_f32(angles.x*DEGREE_TO_RAD);
		float sinY = arm_sin_f32(angles.y*DEGREE_TO_RAD);
		float sinZ = arm_sin_f32(angles.z*DEGREE_TO_RAD);

		float DCM_f32[9] = {cosY*cosZ-sinX*sinY*sinZ,  cosY*cosZ+cosZ*sinX*sinY,  -cosX*sinY,
							-cosX*sinZ, 						cosX*cosZ, 			sinX,
							cosZ*sinY+cosY*sinX*sinZ,  sinY*sinZ-cosY*cosZ*sinX,   cosX*cosY};

		arm_matrix_instance_f32 DCM;
		arm_matrix_instance_f32 current;
		arm_matrix_instance_f32 output;

		float output_f32[3];
		float current_f32[3] = {x,y,z};

		arm_mat_init_f32(&DCM, 		3, 3, DCM_f32);
		arm_mat_init_f32(&current,  3, 1, current_f32);
		arm_mat_init_f32(&output, 	3, 1, output_f32);

		arm_mat_mult_f32(&DCM, &current, &output);

		x = output_f32[0];
		y = output_f32[1];
		z = output_f32[2];
	}
};
