#include "Vector3.h"

void Vector3_DotProduct(Vector3_t a, Vector3_t b, float *d)
{
  *d = a[VEC3_X] * b[VEC3_X] + a[VEC3_Y] * b[VEC3_Y] + a[VEC3_Z] * b[VEC3_Z];
}

void Vector3_CrossProduct(Vector3_t a, Vector3_t b, Vector3_t d)
{
  d[VEC3_X] = a[VEC3_Y] * b[VEC3_Z] - a[VEC3_Z] * b[VEC3_Y];
  d[VEC3_Y] = a[VEC3_Z] * b[VEC3_X] - a[VEC3_X] * b[VEC3_Z];
  d[VEC3_Z] = a[VEC3_X] * b[VEC3_Y] - a[VEC3_Y] * b[VEC3_X];
}
