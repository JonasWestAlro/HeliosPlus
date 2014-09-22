#pragma once

#include "Framework.hpp"
#include "GenericI2C.hpp"

class SensorI2C : public GenericI2C{

	public:
		static SensorI2C& get_instance(void){
				static SensorI2C instance;
				return instance;
			}


	private:
		SensorI2C(): GenericI2C(GENERIC_I2C_TYPE_I2C1_PB6_PB7, 100000){
				}
};
