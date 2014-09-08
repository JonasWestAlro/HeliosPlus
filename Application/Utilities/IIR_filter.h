#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "Framework.h"

/*
 *
 * IIR_filter_t.coeffs must be in the order: b0, b1, b2, b3... a1, a2, a3...
 *
 * */

typedef struct{
	uint16_t   stages;
	float*     coeffs;
	float*	   outputs;
	float* 	   inputs;
}IIR_filter_t;


void IIR_filter_init(IIR_filter_t*, uint8_t stages, float* coeffs, float* inputs, float* outputs);
float IIR_filter_process(IIR_filter_t* filter, float input);

#endif
