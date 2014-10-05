#pragma once

#include "math.h"

class EulerAngle{
public:
	float x;
	float y;
	float z;

	EulerAngle(): x(0.0f), y(0.0f), z(0.0f){};
	EulerAngle(float x_, float y_, float z_): x(x_), y(y_), z(z_){};

	float dot_product(EulerAngle b){
		return x * b.x + y * b.y + z * b.z;
	}

	EulerAngle cross_product(EulerAngle b){
		EulerAngle d;

	  d.x = y * b.z - z * b.y;
	  d.y = z * b.x - x * b.z;
	  d.z = x * b.y - y * b.x;

	  return d;
	}

	void to_degrees(){
		x *= 360/(2*M_PI);
		y *= 360/(2*M_PI);
		z *= 360/(2*M_PI);
	}

};
