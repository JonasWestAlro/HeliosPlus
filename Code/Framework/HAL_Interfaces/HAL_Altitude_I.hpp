#pragma once

#include "Framework.hpp"

class HAL_Altitude_I : public HAL_Interface {
	public:
	uint8_t data_available(void) = 0;
	float   get_altitude(void) = 0;
	uint8_t get_status(void) = 0;
	void 	set_offset(float) = 0;
};

