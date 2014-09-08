#include "BandpassFilter.h"

float BandpassFilter_init(BandpassFilter_t* filter, float lk, float hk){
	filter->highPassK = hk;
	filter->lowPassK = lk;

	filter->lowPassVal = 0;
	filter->highPassVal = 0;
}


float BandpassFilter_update(BandpassFilter_t* filter, float in){

	filter->lowPassVal = filter->lowPassVal + filter->lowPassK * (in - filter->lowPassVal);
	filter->highPassVal = filter->highPassVal + filter->highPassK * (filter->lowPassVal - filter->highPassVal);

	return  filter->lowPassVal - filter->highPassVal;

}
