#pragma once

#include "EulerAngle.hpp"

class Quaternion{
	public:
		float w;
		float x;
		float y;
		float z;

		Quaternion(): x(0.0f), y(0.0f), z(0.0f), w(1.0f){};

		explicit  Quaternion(const EulerAngle& a){
			float cosX2 = cos(x / 2.0f);
			float sinX2 = sin(x / 2.0f);
			float cosY2 = cos(y / 2.0f);
			float sinY2 = sin(y / 2.0f);
			float cosZ2 = cos(z / 2.0f);
			float sinZ2 = sin(z / 2.0f);

			w = cosX2 * cosY2 * cosZ2 + sinX2 * sinY2 * sinZ2;
			x = sinX2 * cosY2 * cosZ2 - cosX2 * sinY2 * sinZ2;
			y = cosX2 * sinY2 * cosZ2 + sinX2 * cosY2 * sinZ2;
			z = cosX2 * cosY2 * sinZ2 - sinX2 * sinY2 * cosZ2;
			normalize();
		}

		EulerAngle to_euler(){
			EulerAngle result;

			float pole = M_PI/2.0f - 0.05f;                           // fix roll near poles with this tolerance

			result.y = asin(2.0f * (w *y - x* z));

			if ((result.y < pole) && (result.y > -pole))
				result.x = atan2(2.0f * (y * z + w * x),
							1.0f - 2.0f * (x* x+y *y));

			result.z = atan2(2.0f * (x*y + w * z),
							1.0f - 2.0f * (y *y + z * z));

			return result;
		}

		void normalize(){
		  float this_length = lenght();

		  if (this_length == 0)
			return;

		  w /= this_length;
		  x /= this_length;
		  y /= this_length;
		  z /= this_length;
		}

		float lenght(){
		  return sqrt(w * w + x * x +
					  y * y + z * z);
		}


		Quaternion conjugate(){
			Quaternion result;

			result.w = w;
			result.x = -x;
			result.y = -y;
			result.z = -z;

			return result;
		}

		Quaternion operator* (const Quaternion& qb)
		{
			  Quaternion qd;

			  EulerAngle va;
			  EulerAngle vb;
			  float dotAB;
			  EulerAngle crossAB;

			  va.x = x;
			  va.y = y;
			  va.z = z;

			  vb.x = qb.x;
			  vb.y = qb.y;
			  vb.z = qb.z;

			  dotAB 	= va.dot_product(vb);
			  crossAB 	= va.cross_product(vb);

			  qd.w= w* qb.w- dotAB;
			  qd.x = w* vb.x + qb.w*va.x + crossAB.x;
			  qd.y = w* vb.y + qb.w* va.y + crossAB.y;
			  qd.z = w* vb.z + qb.w* va.z + crossAB.z;

			  return qd;
		}

};


