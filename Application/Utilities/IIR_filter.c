#include "IIR_filter.h"

void IIR_filter_init(IIR_filter_t* filter, uint8_t stages, float* coeffs, float* inputs, float* outputs){

	filter->stages 	= stages;
	filter->coeffs 	= coeffs;
	filter->outputs = outputs;
	filter->inputs 	= inputs;
}


float IIR_filter_process(IIR_filter_t* filter, float input){
	float output;
	uint16_t i;

	//we add multiply the b0 constant:
	output = filter->coeffs[0]*input;

	//Compute the rest of the coeffs:
	for(i = 1; i<(filter->stages*2)+1; i++){

		//All the b-coeffs comes first:
		if(i <= filter->stages){
			//all the "b" coeffs are added:
			output += filter->coeffs[i]*filter->inputs[i-1];
		}else{
			//all the "a" coeffs are subtracted
			output -= filter->coeffs[i]*filter->outputs[i-filter->stages-1];
		}
	}


	//Now we update the input and output arrays:
	//TODO-JWA: THIS COULD BE OPTIMIZED DRAMACTICLY!
	for(i=filter->stages-1; i>0; i--){
		filter->inputs[i] = filter->inputs[i-1];
	}

	for(i=filter->stages-1; i>0; i--){
		filter->outputs[i] = filter->outputs[i-1];
	}

	filter->inputs[0]  = input;
	filter->outputs[0] = output;

	return output;
}
