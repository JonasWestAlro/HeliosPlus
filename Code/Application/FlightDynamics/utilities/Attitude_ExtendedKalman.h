#ifndef EXTENDED_KALMAN_Haa
#define EXTENDED_KALMAN_Haa

#include "arm_math.h"


#define DT 0.005f // 100Hz

// Q diagonal 3x3 with these elements on diagonal
#define Q1 5.0f
#define Q2 100.0f
#define Q3 0.01f

// R diagonal 2x2 with these elements on diagonal
#define R1 1000.0f
#define R2 1000.0f

struct _ExtendedKalman_data
{
    float x1, x2, x3;
    float p11, p12, p13, p21, p22, p23, p31, p32, p33;
    float q1, q2, q3, r1, r2;
};

typedef float Quaternion_t[4];

typedef struct _ExtendedKalman_data ExtendedKalman_t;

//typedef float* Quaternion_t;

void ExtendedKalman_Init();

void ExtendedKalman_Innovate_Priori(
								Quaternion_t q,
								float Gx,
								float Gy,
								float Gz,
								float dt);

void ExtendedKalman_Innovate_Stage1(Quaternion_t q,
							 float Ax,
							 float Ay,
							 float Az);

void ExtendedKalman_Innovate_Stage2(Quaternion_t q,
		 	 	 	 	 	 float Mx,
							 float My,
							 float Mz);

void ExtendedKalman_copy_matrix(arm_matrix_instance_f32* from,
							    arm_matrix_instance_f32* to);


void Quaternion_Normalize(Quaternion_t q);
inline float Quaternion_Norm(Quaternion_t q);

#endif
