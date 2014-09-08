#ifndef BANDPASSFILTER_H
#define BANDPASSFILTER_H
#include "Framework.h"

typedef struct{
	float lowPassK;
	float highPassK;
	float lowPassVal;
	float highPassVal;
}BandpassFilter_t;

float BandpassFilter_init(BandpassFilter_t*, float, float);
float BandpassFilter_update(BandpassFilter_t* filter, float in);


#endif
