#include "ComplementaryFilter.h"

float complementaryFilter_update(float a, float value1, float value2){
	return a*(value1) + (1.0-a)*(value2);
}
