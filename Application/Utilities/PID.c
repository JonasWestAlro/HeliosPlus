#include "PID.h"

void  PID_init(PID_t* pid, float p, float i, float d, float windup){

	pid->P = p;
	pid->I = i;
	pid->D = d;
	pid->timestamp 		 = 0.0;
	pid->lastError 		 = 0.0;
	pid->integratedError = 0.0;
	pid->windupGuard 	 = windup;
}
float debug_dt;

float PID_update(PID_t* pid, float currentValue, float setPoint){
	float dTerm = 0;
	float iTerm = 0;
	float pTerm = 0;

	//Compute the time since last calculation:
	float dt = Framework_GetTimeSinceSec(pid->timestamp);
	debug_dt = dt;
	pid->timestamp = Framework_GetTimeStamp();

	//Compute the error:
	float error = setPoint - currentValue;
	pTerm = error*pid->P;

	//Integrate the error if the dt is < 50ms:
	if(dt < 0.05){
		pid->integratedError += error * dt;
		pid->integratedError = PID_constrain(pid->integratedError, pid->windupGuard);
	}

	iTerm = pid->integratedError*pid->I;

	//Compute the change in process variable:
	if(dt < 0.05){
		dTerm = pid->D * (pid->lastError - currentValue)/dt;
	}
	pid->lastError = currentValue;


	return pTerm - iTerm + dTerm;
}

void PID_resetIntegrator(PID_t* pid){
	pid->integratedError = 0.0;
}

void PID_setKi(PID_t* pid, float ki){
	pid->I = ki;
}

float PID_constrain(float value, float constrainValue){
	if(value > constrainValue){
		return constrainValue;
	}
	if(value < -constrainValue){
		return -constrainValue;
	}

	return value;
}
