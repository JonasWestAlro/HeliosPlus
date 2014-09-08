#include "Attitude_ExtendedKalman.h"

#include "Quaternion.h"

#define T 0.0005

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

void ExtendedKalman_Init(){
	//Parameters
	arm_mat_init_f32(&Pk, 4, 4, Pk_f32);
	arm_mat_init_f32(&R,  3, 3, R_f32);
	arm_mat_init_f32(&Qk, 4, 4, Qk_f32);
	arm_mat_init_f32(&I,  4, 4, I_f32);
	arm_mat_init_f32(&x,  2, 1, x_f32);

	//Helpers:
	arm_mat_init_f32(&Ak,       4, 4, Ak_f32);
	arm_mat_init_f32(&Ak_trans, 4, 4, Ak_trans_f32);

	arm_mat_init_f32(&S,   	  3, 3, S_f32);
	arm_mat_init_f32(&K_k1,   4, 3, K_k1_f32);
	arm_mat_init_f32(&K_k2,   4, 3, K_k2_f32);
	arm_mat_init_f32(&h1_q,   3, 1, h1_q_f32);
	arm_mat_init_f32(&q_corr, 4, 1, q_corr_f32);
	arm_mat_init_f32(&q_corr_2, 4, 1, q_corr_2_f32);


	//Temps
	arm_mat_init_f32(&temp4x4,  4, 4, temp4x4_f32);
	arm_mat_init_f32(&temp4x4_2,  4, 4, temp4x4_2_f32);
	arm_mat_init_f32(&temp3x4,  3, 4, temp3x4_f32);
	arm_mat_init_f32(&temp4x3,  4, 3, temp4x3_f32);
	arm_mat_init_f32(&temp3x3,  3, 3, temp3x3_f32);
	arm_mat_init_f32(&temp3x1,  3, 1, temp3x1_f32);
	arm_mat_init_f32(&temp4x1,  4, 1, temp4x1_f32);
	arm_mat_init_f32(&null4x4,  4, 4, null4x4_f32);


	//arm_mat_scale_f32(&R, 0.0001, &temp3x3);
	//ExtendedKalman_copy_matrix(&R, &temp3x3);

	arm_mat_scale_f32(&Qk, 0.0001, &temp4x4);
	ExtendedKalman_copy_matrix(&Qk, &temp4x4);
}


void ExtendedKalman_Innovate_Priori(
								Quaternion_t quaternion,
								float Gx,
								float Gy,
								float Gz,
								float dt){

		arm_matrix_instance_f32 q;
		arm_mat_init_f32(&q, 4, 1, quaternion);

		//---------------------- PREDICTION -----------------------------
		//1. Obtain the angular velocities:
		float wx = (Gx*2*PI)/360.0;
		float wy = (Gy*2*PI)/360.0;
		float wz = (Gz*2*PI)/360.0;

		//2. Calculate the discrete time state transition matrix:
		float32_t omega_f32[16] = {
				0, -wx, -wy, -wz,
				wx, 0,   wz, -wy,
				wy, -wz, 0,   wx,
				wz, wy, -wx,  0};

		arm_matrix_instance_f32 omega;      /* Matrix Omega Instance */
		arm_mat_init_f32(&omega, 4, 4, omega_f32);

		//Ak = I + 0.5*omega*T;
		arm_mat_scale_f32(&omega, 0.5*dt, &temp4x4);
		arm_mat_add_f32(&I, &temp4x4, &Ak);

		//3. Calculation of the a priori system state:
		// q = Ak*q
		arm_mat_mult_f32(&Ak, &q, &temp4x1);

		//memcpy((void*)quaternion, (void*)temp4x1_f32, 4);
		quaternion[0] = temp4x1_f32[0];
		quaternion[1] = temp4x1_f32[1];
		quaternion[2] = temp4x1_f32[2];
		quaternion[3] = temp4x1_f32[3];

		//4. Calculation of the a proiri noise covariance matrix:
		//   Pk = Ak*Pk*Ak.' + Qk;
		arm_mat_trans_f32(&Ak, &Ak_trans);

		//temp = Ak*Pk
		arm_mat_mult_f32(&Ak, &Pk, &temp4x4);
		//Pk = temp*Ak.'
		arm_mat_mult_f32(&temp4x4, &Ak_trans, &Pk);
		//temp = Pk + Qk:
		arm_mat_add_f32(&Pk, &Qk, &temp4x4);
		//Pk = temp;
		ExtendedKalman_copy_matrix(&Pk, &temp4x4);
}

void ExtendedKalman_Innovate_Stage1(Quaternion_t quaternion,
							 float Ax,
							 float Ay,
							 float Az){

	arm_matrix_instance_f32 q;
	arm_mat_init_f32(&q, 4, 1, quaternion);

	//----------------------   MEASUREMENT UPDATE   -----------------------------
	//1: Calculation of the Jacobian matrix:
		float32_t H_k1_f32[16] = {
				-2*quaternion[2],  2*quaternion[3], -2*quaternion[0], 2*quaternion[1],
				 2*quaternion[1],  2*quaternion[0],  2*quaternion[3], 2*quaternion[2],
				 2*quaternion[0], -2*quaternion[1], -2*quaternion[2], 2*quaternion[3]
		};
		arm_matrix_instance_f32 H_k1;      /* Matrix Omega Instance */
		arm_mat_init_f32(&H_k1, 3, 4, H_k1_f32);

	//2: Calculate the Kalman Gain:
		/*    S = inv(H_k1 * Pk * H_k1.' + V_k1*R_k1*V_k1.');
			  K_k1 = Pk * H_k1.' * S;
		*/

		//temp3x3 = H_k1 * Pk * H_k1.'
		arm_mat_trans_f32(&H_k1, &temp4x3);
		arm_mat_mult_f32(&H_k1, &Pk, &temp3x4);
		arm_mat_mult_f32(&temp3x4, &temp4x3, &temp3x3);

		//temp3x3 = inv(temp3x3 + R)   ( = S!!)
		arm_mat_add_f32(&temp3x3, &R, &S);
		arm_mat_inverse_f32(&S, &temp3x3);

		//K_k1 = Pk * H_k1.'    (temp4x3 = H_k1.')
		arm_mat_mult_f32(&Pk, &temp4x3, &K_k1);
		arm_mat_mult_f32(&K_k1, &temp3x3, &temp4x3);

		ExtendedKalman_copy_matrix(&K_k1, &temp4x3);

		//3: Reading of the accelerometer data:
		float32_t z_f32[3] = {
				Ax,
				Ay,
				Az,
		};
		arm_matrix_instance_f32 z;      /* Matrix Omega Instance */
		arm_mat_init_f32(&z, 3, 1, z_f32);

   //4: Calculation of h1(q):
		float32_t h1_q_f32[3] = {
				2*quaternion[1]*quaternion[3]-2*quaternion[0]*quaternion[2],
				2*quaternion[0]*quaternion[1]+2*quaternion[2]*quaternion[3],
				quaternion[0]*quaternion[0] - quaternion[1]*quaternion[1]- quaternion[2]*quaternion[2] + quaternion[3]*quaternion[3]
		};
		arm_matrix_instance_f32 h1_q;      /* Matrix Omega Instance */
		arm_mat_init_f32(&h1_q, 3, 1, h1_q_f32);

    //5: Calculation of the correction factor
    	//q_corr = K_k1*(z - h1_q);
		arm_mat_sub_f32(&z, &h1_q, &temp3x1);
		arm_mat_mult_f32(&K_k1, &temp3x1, &q_corr);
		q_corr.pData[3] = 0;

	//6: Calculation of the a posteriori state estimation
	    //q = q + q_1;
		arm_mat_add_f32(&q, &q_corr, &temp4x1);
		ExtendedKalman_copy_matrix(&q, &temp4x1);

	//7: Calculation of a aposteriori error covariance matrix
	    //Pk = (I - K_k1 * H_k1)*Pk;
		arm_mat_mult_f32(&K_k1, &H_k1, &temp4x4);
		arm_mat_sub_f32(&I, &temp4x4, &temp4x4_2);

		arm_mat_mult_f32(&temp4x4_2, &Pk, &temp4x4);
		ExtendedKalman_copy_matrix(&Pk, &temp4x4);

}

void ExtendedKalman_Innovate_Stage2(Quaternion_t quaternion,
							 float Mx,
							 float My,
							 float Mz){

	arm_matrix_instance_f32 q;
	arm_mat_init_f32(&q, 4, 1, quaternion);

	//----------------------   MEASUREMENT UPDATE   -----------------------------
	//1: Calculation of the Jacobian matrix:
		float32_t H_k2_f32[16] = {
				 2*quaternion[3],  2*quaternion[2],  2*quaternion[1],  2*quaternion[0],
				 2*quaternion[0], -2*quaternion[1], -2*quaternion[2], -2*quaternion[3],
				-2*quaternion[1], -2*quaternion[0],  2*quaternion[3],  2*quaternion[2]
		};
		arm_matrix_instance_f32 H_k2;      /* Matrix Omega Instance */
		arm_mat_init_f32(&H_k2, 3, 4, H_k2_f32);

	//2: Calculate the Kalman Gain:
		/*    S = inv(H_k2 * Pk * H_k2.' + V_k2 * R_k2 * V_k2.');
			  K_k1 = Pk * H_k2.' * S;
		*/

		//temp3x3 = H_k2 * Pk * H_k2.'
		arm_mat_trans_f32(&H_k2, &temp4x3);
		arm_mat_mult_f32(&H_k2, &Pk, &temp3x4);
		arm_mat_mult_f32(&temp3x4, &temp4x3, &temp3x3);

		//temp3x3 = inv(temp3x3 + R)   ( = S!!)
		arm_mat_add_f32(&temp3x3, &R, &S);
		arm_mat_inverse_f32(&S, &temp3x3);

		//K_k2 = Pk * H_k2.'    (temp4x3 = H_k2.')
		arm_mat_mult_f32(&Pk, &temp4x3, &K_k2);
		arm_mat_mult_f32(&K_k2, &temp3x3, &temp4x3);

		ExtendedKalman_copy_matrix(&K_k2, &temp4x3);

	//3: Reading of the magnetometer data:
		float32_t z_k2_f32[3] = {
				Mx,
				My,
				Mz,
		};
		arm_matrix_instance_f32 z_k2;      /* Matrix Omega Instance */
		arm_mat_init_f32(&z_k2, 3, 1, z_k2_f32);

   //4: Calculation of h2(q):
		float32_t h2_q_f32[3] = {
				2*quaternion[1]*quaternion[2] + 2*quaternion[0]*quaternion[3],
				  quaternion[0]*quaternion[0] - quaternion[1]*quaternion[1]- quaternion[2]*quaternion[2] - quaternion[3]*quaternion[3],
				2*quaternion[2]*quaternion[3] - 2*quaternion[0]*quaternion[1]
		};
		arm_matrix_instance_f32 h2_q;      /* Matrix Omega Instance */
		arm_mat_init_f32(&h2_q, 3, 1, h2_q_f32);

    //5: Calculation of the correction factor
    	//q_corr_2 = K_k2*(z_k2 - h2_q);
		arm_mat_sub_f32(&z_k2, &h2_q, &temp3x1);
		arm_mat_mult_f32(&K_k2, &temp3x1, &q_corr_2);
		q_corr_2.pData[1] = 0;
		q_corr_2.pData[2] = 0;

    //6: Calculation of the a posteriori state estimation
	    //q = q + q_2;
		arm_mat_add_f32(&q, &q_corr_2, &temp4x1);
		ExtendedKalman_copy_matrix(&q, &temp4x1);

	//7: Calculation of a aposteriori error covariance matrix
	    //Pk = (I - K_k2 * H_k2)*Pk;
		arm_mat_mult_f32(&K_k2, &H_k2, &temp4x4);
		arm_mat_sub_f32(&I, &temp4x4, &temp4x4_2);

		arm_mat_mult_f32(&temp4x4_2, &Pk, &temp4x4);
		ExtendedKalman_copy_matrix(&Pk, &temp4x4);
}

void ExtendedKalman_copy_matrix(arm_matrix_instance_f32* to,
							    arm_matrix_instance_f32* from){
	uint8_t i;
	for(i=0; i<from->numCols*from->numRows; i++){
		to->pData[i] = from->pData[i];
	}

}
