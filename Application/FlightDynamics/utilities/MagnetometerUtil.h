#ifndef MAGNETOMETER_UTIL
#define MAGNETOMETER_UTIL

#include "math.h"
#include "Quaternion.h"
#include "mavlink.h"

float magnetometerUtil_getYaw(float* raw, Quaternion_t q);

#endif
