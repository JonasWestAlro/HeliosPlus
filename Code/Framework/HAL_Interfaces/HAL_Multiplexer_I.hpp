#pragma once

#include "Framework.hpp"

typedef enum{
	FLIGHTCONTROLLER = 0,
	APPLICATIONPROCCESOR
}MULTIPLEXER_CHANNEL;

class HAL_Multiplexer_I : public HAL_Interface {
	public:
	virtual void set_channel(MULTIPLEXER_CHANNEL) = 0;
};
