#ifndef PID_H
#define PID_H

#include "Framework.h"

typedef struct{
	  float P, I, D;
	  float lastError;
	  float integratedError;
	  float windupGuard;
	  uint32_t timestamp;
}PID_t;

void  PID_init(PID_t* pid, float p, float i, float d, float windup);
float PID_update(PID_t* pid, float currentValue, float setPoint);
void  PID_resetIntegrator(PID_t* pid);
void PID_setKi(PID_t* pid, float ki);
float PID_constrain(float value, float constrainValue);

#endif
