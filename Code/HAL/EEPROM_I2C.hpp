#pragma once

#include "Framework.hpp"
#include "GenericI2C.hpp"

class EepromI2C : public GenericI2C{

	public:
		static EepromI2C& get_instance(void){
				static EepromI2C instance;
				return instance;
			}


	private:
		EepromI2C(): GenericI2C(GENERIC_I2C_TYPE_I2C3_PA8_PC9, 100000){
				}
};
