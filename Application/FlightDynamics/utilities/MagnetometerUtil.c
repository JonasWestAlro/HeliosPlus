#include "MagnetometerUtil.h"

#define X 0
#define Y 1
#define Z 2

float magnetometerUtil_getYaw(float* raw, Quaternion_t q){
	float newMagYaw;
	static float m_lastYaw = 0;

	/* NOT USED
	Quaternion_t qConjugate;
	float qMag[4];
	qMag[QUAT_W] = 0;
	qMag[QUAT_X] = raw[VEC3_X];
	qMag[QUAT_Y] = raw[VEC3_Y];
	qMag[QUAT_Z] = raw[VEC3_Z];
	*/

	//Quaternion_Conjugate(q, qConjugate);
	//Quaternion_Multiply(qMag, qConjugate, temp1);
	//Quaternion_Multiply(q, temp1, qMag);

	float angles[3];
	mavlink_quaternion_to_euler(q, &angles[X], &angles[Y], &angles[Z]);

	float cosPitch = cos(angles[Y]);
	float sinPitch = sin(angles[Y]);
	float cosRoll  = cos(angles[X]);
	float sinRoll  = sin(angles[X]);

	 //Tilt compensering:
	float mAngleX = raw[X]*cosPitch + raw[Y]*sinRoll*sinPitch + raw[Z]*cosRoll*sinPitch;    // Tilt compensated Magnetic filed X:
	float mAngleY = raw[Y]*cosRoll - raw[Z]*sinRoll;   										// Tilt compensated Magnetic filed Y:

	 newMagYaw = -atan2(mAngleY, mAngleX);
	//newMagYaw = -atan2(qMag[QUAT_Y], qMag[QUAT_X]);

	if (newMagYaw < 0)
		newMagYaw = 2.0f * (float)M_PI + newMagYaw;               // need 0 <= newMagYaw <= 2*PI

	if (newMagYaw > (float)M_PI)
		newMagYaw -= 2.0f * (float)M_PI;

	if (newMagYaw != newMagYaw) {                                 // check for nAn
	    return m_lastYaw;                                         // just ignore in this case
	}

	m_lastYaw = newMagYaw;
	return newMagYaw;



	/*newYaw = m_lastYaw + gyroZ*dt*(2*M_PI/360.0);				  // compute new yaw from change

	if (newYaw > (2.0f * (float)M_PI))                            // need 0 <= newYaw <= 2*PI
		newYaw -= 2.0f * (float)M_PI;
	if (newYaw < 0)
		newYaw += 2.0f * (float)M_PI;

	deltaMagYaw = newMagYaw - newYaw;                             // compute difference

	if (deltaMagYaw >= (float)M_PI)
	deltaMagYaw = deltaMagYaw - 2.0f * (float)M_PI;
	if (deltaMagYaw <= -(float)M_PI)
	deltaMagYaw = (2.0f * (float)M_PI + deltaMagYaw);

	newYaw += deltaMagYaw/4;                                      // apply some of the correction

	if (newYaw > (2.0f * (float)M_PI))				              // need 0 <= newYaw <= 2*PI
	newYaw -= 2.0f * (float)M_PI;
	if (newYaw < 0)
	newYaw += 2.0f * (float)M_PI;

	m_lastYaw = newYaw;

	if (newYaw > (float)M_PI)
	newYaw -= 2.0f * (float)M_PI;

	return newYaw;                            // fill in output yaw value

	//MPUQuaternionEulerToQuaternion(m_fusedEulerPose, m_fusedQuaternion);*/
}




/*
	float angles[3];
	mavlink_quaternion_to_euler(q, &angles[X], &angles[Y], &angles[Z]);

	 float cosPitch = cos(angles[X]);
	 float sinPitch = sin(angles[X]);
	 float cosRoll  = cos(angles[Y]);
	 float sinRoll  = sin(angles[Y]);

	 //Tilt compensering:
	 float mAngleX = raw[X]*cosPitch + raw[Y]*sinRoll*sinPitch + raw[Z]*cosRoll*sinPitch;   // Tilt compensated Magnetic filed X:
	 float mAngleY = raw[Y]*cosRoll - raw[Z]*sinRoll;   									// Tilt compensated Magnetic filed Y:

 //Yaw udregnes:
 float magYaw = (atan2(-mAngleY,mAngleX)) * (360/(3.14*2)); */
