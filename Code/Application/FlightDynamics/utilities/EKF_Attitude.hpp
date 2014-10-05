#pragma once

#include "Framework.hpp"
#include "arm_math.h"
#include "Quaternion.hpp"

#define DT 0.005f // 100Hz

// Q diagonal 3x3 with these elements on diagonal
#define Q1 5.0f
#define Q2 100.0f
#define Q3 0.01f

// R diagonal 2x2 with these elements on diagonal
#define R1 1000.0f
#define R2 1000.0f


class EKF_Attitude{
public:
	EKF_Attitude();

	void innovate_priori(Quaternion& q,
						float Gx,
						float Gy,
						float Gz);

	void innovate_stage1(Quaternion& q,
								 float Ax,
								 float Ay,
								 float Az);

	void innovate_stage2(Quaternion& q,
			 	 	 	 	 	 float Mx,
								 float My,
								 float Mz);


private:
    float x1, x2, x3;
    float p11, p12, p13, p21, p22, p23, p31, p32, p33;
    float q1, q2, q3, r1, r2;

	void copy_matrix(arm_matrix_instance_f32* from,
					 arm_matrix_instance_f32* to);

	/*******************************************************************************
	 *                           PARAMETER DEFINES
	 *******************************************************************************/
	float32_t Pk_f32[16] =
	{
	  0.125,   0.003,   0.003,  0.003,
	  0.003,   0.125,   0.003,  0.003,
	  0.003,   0.003,   0.125,  0.003,
	  0.003,   0.003,   0.003,  0.125,
	};

	/* Accelerometer Noise Covariance */
	float32_t R_f32[9] =
	{
		1000,     0,    0.0,
		0.0,   1000,   -0.0,
		0.0,   -0.0,    1000
	};

	/* Gyro Noise Covariance! */
	float32_t Qk_f32[16] =
	{
		1.000,   0.000,   0.000,  0.000,
		0.000,   1.000,   0.000,  0.000,
		0.000,   0.000,   1.000,  0.000,
		0.000,   0.000,   0.000,  1.000,
	};

	float32_t I_f32[16] = {
		1.000, 0.000, 0.000, 0.000,
		0.000, 1.000, 0.000, 0.000,
		0.000, 0.000, 1.000, 0.000,
		0.000, 0.000, 0.000, 1.000
	};

	float32_t x_f32[2] = {0, 0};


	arm_matrix_instance_f32 Pk;     /* Matrix Pk Instance */
	arm_matrix_instance_f32 R;      /* Matrix R Instance */
	arm_matrix_instance_f32 Qk;     /* Matrix Qk Instance */
	arm_matrix_instance_f32 I;      /* Matrix I Instance */
	arm_matrix_instance_f32 x;      /* Matrix x Instance */

	/*******************************************************************************
	 *                           HELPER DEFINES
	 *******************************************************************************/
	float32_t Ak_f32[16] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t Ak_trans_f32[16] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t H_k1_f32[12] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t H_k2_f32[12]=
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t S_f32[9] =
	{
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
	};

	float32_t K_k1_f32[12] =
	{
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000
	};

	float32_t K_k2_f32[12] =
	{
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000
	};

	float32_t h1_q_f32[3] =
	{
		0.000,
		0.000,
		0.000
	};

	float32_t q_corr_f32[4] =
	{
		0.000,
		0.000,
		0.000,
		0.000
	};

	float32_t q_corr_2_f32[4] =
	{
		0.000,
		0.000,
		0.000,
		0.000
	};

	arm_matrix_instance_f32 Ak;        /* Matrix Ak Instance */
	arm_matrix_instance_f32 Ak_trans;  /* Matrix Ak_trans Instance */

	arm_matrix_instance_f32 S;  		/* Matrix S Instance */
	arm_matrix_instance_f32 K_k1;  		/* Matrix K_k1 Instance */
	arm_matrix_instance_f32 K_k2;  		/* Matrix K_k2 Instance */
	arm_matrix_instance_f32 h1_q;  		/* Matrix h1_q Instance */
	arm_matrix_instance_f32 q_corr;  	/* Matrix q_corr Instance */
	arm_matrix_instance_f32 q_corr_2;  	/* Matrix q_corr_2 Instance */

	/*******************************************************************************
	 *                           TEMP DEFINES
	 *******************************************************************************/
	float32_t temp4x4_f32[16] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t temp4x4_2_f32[16] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};


	float32_t temp3x4_f32[12] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};


	float32_t temp4x3_f32[12] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};

	float32_t temp3x3_f32[9] =
	{
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
		0.000,   0.000,   0.000,
	};

	float32_t temp3x1_f32[3] =
	{
		0.000,
		0.000,
		0.000,
	};


	float32_t temp4x1_f32[4] =
	{
		0.000,
		0.000,
		0.000,
		0.000,
	};

	float32_t null4x4_f32[16] =
	{
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
		0.000,   0.000,   0.000,  0.000,
	};


	arm_matrix_instance_f32 temp4x4;   /* Matrix temp4x4 Instance */
	arm_matrix_instance_f32 temp4x4_2;   /* Matrix temp4x4 Instance */
	arm_matrix_instance_f32 temp3x4;   /* Matrix temp4x1 Instance */
	arm_matrix_instance_f32 temp4x3;   /* Matrix temp4x1 Instance */
	arm_matrix_instance_f32 temp3x3;   /* Matrix temp4x1 Instance */
	arm_matrix_instance_f32 temp3x1;   /* Matrix temp4x1 Instance */
	arm_matrix_instance_f32 temp4x1;   /* Matrix temp4x1 Instance */
	arm_matrix_instance_f32 null4x4;   /* Matrix temp4x1 Instance */
};

