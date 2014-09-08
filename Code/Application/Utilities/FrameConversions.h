#ifndef FRAMECONVERSIONS_H
#define FRAMECONVERSIONS_H

#include "Vector3.h"
#include "arm_math.h"

float frameConvert_bodyToEarthXYZ(Vector3_t body, Vector3_t earth, Vector3_t output);
float frameConvert_bodyToEarthZYX(Vector3_t body, Vector3_t earth, Vector3_t output);

#endif
