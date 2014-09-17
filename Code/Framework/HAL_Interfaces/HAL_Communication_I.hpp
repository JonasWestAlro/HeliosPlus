#pragma once

#include "Framework.hpp"

class HAL_Communication_I : public HAL_Interface {
	public:
	virtual uint16_t data_available(void) = 0;
	virtual bool  	 receive(uint8_t*) = 0;
	virtual bool  	 send(uint8_t) = 0;
	virtual uint16_t put(uint8_t*, uint16_t) = 0;
	virtual uint16_t transmit(void) = 0;
};
