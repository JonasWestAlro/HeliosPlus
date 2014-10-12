#pragma once

#include "HAL_Interface.hpp"

class HAL_Communication_I : public HAL_Interface {
	public:
	virtual uint16_t data_available(void) = 0;
	virtual bool  	 receive(uint8_t*) = 0;
	virtual bool  	 send(uint8_t) = 0;
	virtual bool 	 put(const char*, uint16_t) = 0;
	virtual bool 	 put(const char*) = 0;
	virtual bool 	 transmit(void) = 0;

	virtual bool send_number(uint32_t) = 0;
	virtual bool send_number(float) = 0;
};
