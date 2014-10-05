#pragma once

#include <initializer_list>

template<unsigned int N>
class IIR_Filter{
public:
	IIR_Filter(){}

	IIR_Filter(std::initializer_list<float> list){
		set_coeffs(list);
	}

	void set_coeffs(std::initializer_list<float> list){
		uint8_t i = 0;

		for(auto elem : list ){
			coeffs[i] = elem;
			i++;
			if(i == (N*2+1)) break;
		}
	}

	float process(float input){
		float output;
		uint16_t i;

		//we add multiply the b0 constant:
		output = coeffs[0]*input;

		//Compute the rest of the coeffs:
		for(i = 1; i<(N*2)+1; i++){

			//All the b-coeffs comes first:
			if(i <= N){
				//all the "b" coeffs are added:
				output += coeffs[i]*inputs[i-1];
			}else{
				//all the "a" coeffs are subtracted
				float cons = coeffs[i];
				float value = outputs[i-N-1];
				float output_temp = output;
				output -= coeffs[i]*outputs[i-N-1];
			}
		}


		//Now we update the input and output arrays:
		//TODO-JWA: THIS COULD BE OPTIMIZED DRAMACTICLY!
		for(i=N-1; i>0; i--){
			inputs[i] = inputs[i-1];
		}

		for(i=N-1; i>0; i--){
			outputs[i] = outputs[i-1];
		}

		inputs[0]  = input;
		outputs[0] = output;

		return output;
	}

private:
	float      coeffs[N*2+1] = {0};
	float	   outputs[N] = {0};
	float 	   inputs[N] = {0};

};
