#include "AccelerometerUtil.h"
#include <math.h>

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

float accelerometerUtil_getRoll(float* raw){
    float nominator;
	float denominator = sqrt(pow((raw[Z_AXIS]),2) + (pow(raw[X_AXIS],2)));

    if(denominator != 0){
    	nominator = raw[Y_AXIS];
    	return atan2(nominator, denominator)*57.29577;
    }
    return 0;
}

float accelerometerUtil_getPitch(float* raw){
    float nominator;
 	float denominator = sqrt(pow((raw[Z_AXIS]),2) + (pow(raw[Y_AXIS],2)));

	if(denominator != 0){
		nominator = raw[X_AXIS];
		return atan2(nominator, denominator)*57.29577;
	}
	return 0;
}

float accelerometerUtil_getGravity(float* raw){
	return sqrt(pow(raw[0],2) + pow(raw[1],2) + pow(raw[2],2));
}
