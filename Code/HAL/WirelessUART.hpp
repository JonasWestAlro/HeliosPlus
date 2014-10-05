#pragma once

#include "Framework.hpp"
#include "GenericUART.hpp"
#include "Multiplexer.hpp"

class WirelessUART : public GenericUART{

	public:
	WirelessUART():
			GenericUART(GENERIC_UART_TYPE_USART3_PB10_PB11, 115200){
		multiplexer.set_channel(FLIGHTCONTROLLER);
	}


	private:
		Multiplexer multiplexer;

};
