#pragma once

#include "Framework.hpp"

class HAL_Communication_I : public HAL_Interface {
	public:
	virtual uint8_t data_available(void) = 0;
	virtual uint8_t receive(void) = 0;
	virtual uint8_t send(uint8_t) = 0;
	virtual uint8_t put(uint8_t*, uint16_t) = 0;
	virtual uint8_t transmit(void) = 0;
};
