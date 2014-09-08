#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <math.h>

#define	DEGREE_TO_RAD		((float)M_PI / 180.0f)
#define	RAD_TO_DEGREE		(180.0f / (float)M_PI)

#define VEC3_X		0										// x offset
#define VEC3_Y		1										// y offset
#define VEC3_Z		2										// z offset

typedef float Vector3_t[3];

typedef struct{
	float x;
	float y;
	float z;
}Vector3f_t;

void Vector3_DotProduct(Vector3_t a, Vector3_t b, float *d);
void Vector3_CrossProduct(Vector3_t a, Vector3_t b, Vector3_t d);

#endif /* MPUVECTOR3_H_ */
