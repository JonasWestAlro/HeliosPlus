#include "FrameConversions.h"

#define X 0
#define Y 1
#define Z 2

// http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions
// x,y,z coordinate
float frameConvert_bodyToEarthXYZ(Vector3_t body, Vector3_t earth, Vector3_t output){
	float cosX = arm_cos_f32(earth[X]*DEGREE_TO_RAD);
	float cosY = arm_cos_f32(earth[Y]*DEGREE_TO_RAD);
	float cosZ = arm_cos_f32(earth[Z]*DEGREE_TO_RAD);
	float sinX = arm_sin_f32(earth[X]*DEGREE_TO_RAD);
	float sinY = arm_sin_f32(earth[Y]*DEGREE_TO_RAD);
	float sinZ = arm_sin_f32(earth[Z]*DEGREE_TO_RAD);

	float DCM_f32[9] = {cosY*cosZ-sinX*sinY*sinZ,  cosY*cosZ+cosZ*sinX*sinY,  -cosX*sinY,
						-cosX*sinZ, 						cosX*cosZ, 			sinX,
						cosZ*sinY+cosY*sinX*sinZ,  sinY*sinZ-cosY*cosZ*sinX,   cosX*cosY};

	arm_matrix_instance_f32 DCM;
	arm_matrix_instance_f32 bodyframe;
	arm_matrix_instance_f32 earthframe;

	arm_mat_init_f32(&DCM, 		  3, 3, DCM_f32);
	arm_mat_init_f32(&bodyframe,  3, 1, body);
	arm_mat_init_f32(&earthframe, 3, 1, output);

	arm_mat_mult_f32(&DCM, &bodyframe, &earthframe);
}

// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// z,y,x coordinate
float frameConvert_bodyToEarthZYX(Vector3_t body, Vector3_t earth, Vector3_t output)
{
	float cosX = arm_cos_f32(earth[X]*DEGREE_TO_RAD);
	float cosY = arm_cos_f32(earth[Y]*DEGREE_TO_RAD);
	float cosZ = arm_cos_f32(earth[Z]*DEGREE_TO_RAD);
	float sinX = arm_sin_f32(earth[X]*DEGREE_TO_RAD);
	float sinY = arm_sin_f32(earth[Y]*DEGREE_TO_RAD);
	float sinZ = arm_sin_f32(earth[Z]*DEGREE_TO_RAD);

	float Rzyx_f32[9] = {cosY*cosZ,  -cosX*sinZ+sinX*sinY*cosZ,  sinX*sinZ+cosX*sinY*cosZ,
						 cosY*sinZ, 	cosX*cosZ+sinX*sinY*sinZ, 	-sinX*cosZ+cosX*sinY*sinZ,
						 -sinY,  	sinX*cosY,   				cosX*cosY};

	arm_matrix_instance_f32 Rzyx;
	arm_matrix_instance_f32 bodyframe;
	arm_matrix_instance_f32 earthframe;

	arm_mat_init_f32(&Rzyx, 	  3, 3, Rzyx_f32);
	arm_mat_init_f32(&bodyframe,  3, 1, body);
	arm_mat_init_f32(&earthframe, 3, 1, output);

	arm_mat_mult_f32(&Rzyx, &bodyframe, &earthframe);
}
