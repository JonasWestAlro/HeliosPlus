#pragma once

#include "Framework.hpp"

class PID{
public:

	PID(){
			set_constants(0,0,0,0);
	}

	PID(float p, float i, float d, float windup){
		set_constants(p,i,d,windup);
	}

	void set_constants(float p, float i, float d, float windup){
		P = p;
		I = i;
		D = d;
		timestamp 		 = 0.0;
		last_error 		 = 0.0;
		integrated_error  = 0.0;
		windup_guard 	 = windup;
	}

	float 	update(float current_value, float set_point){
		float d_term = 0;
		float i_term = 0;
		float p_term = 0;

		//Compute the time since last calculation:
		float dt = Time.get_time_since_sec(timestamp);
		timestamp = Time.get_timestamp();

		//Compute the error:
		float error = set_point - current_value;
		p_term = error*P;

		//Integrate the error if the dt is < 50ms:
		if(dt < 0.05){
			integrated_error += error * dt;
			integrated_error = constrain(integrated_error, windup_guard);
		}

		i_term = integrated_error*I;

		//Compute the change in process variable:
		if(dt < 0.05){
			d_term = D * (last_error - current_value)/dt;
		}
		last_error = current_value;


		return p_term - i_term + d_term;
	}

	void  	reset_integrator(){
		integrated_error = 0.0;
	}
	void 	set_ki(float ki){I = ki;}
	void 	set_kp(float kp){P = kp;}
	void 	set_kd(float kd){D = kd;}

private:
	float P, I, D;
	float last_error;
	float integrated_error;
	float windup_guard;
	uint32_t timestamp;

	float constrain(float value, float constrain_value){
		if(value > constrain_value){
			return constrain_value;
		}
		if(value < -constrain_value){
			return -constrain_value;
		}
		return value;
	}
};
