#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"

//typedef float* Quaternion_t;
typedef float Quaternion_t[4];

#define QUAT_W		0										// scalar offset
#define QUAT_X		1										// x offset
#define QUAT_Y		2										// y offset
#define QUAT_Z		3										// z offset

void Quaternion_Normalize(Quaternion_t q);
void Quaternion_QuaternionToEuler(const Quaternion_t q, Vector3_t v);
void Quaternion_EulerToQuaternion(const Vector3_t v, Quaternion_t q);
void Quaternion_Conjugate(const Quaternion_t s, Quaternion_t d);
void Quaternion_Multiply(const Quaternion_t qa, const Quaternion_t qb, Quaternion_t qd);

inline float Quaternion_Norm(Quaternion_t q);
void Quaternion_Normalize(float *q);
#endif
