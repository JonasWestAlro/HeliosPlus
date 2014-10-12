#pragma once

#include "HAL_Interface.hpp"

class HAL_Altitude_I : public HAL_Interface {
	public:
	virtual uint8_t data_available(void) = 0;
	virtual float   get_altitude(void) = 0;
	virtual uint8_t get_status(void) = 0;
	virtual void 	set_offset(float) = 0;
};

