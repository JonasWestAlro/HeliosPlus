#pragma once

#include "Framework.hpp"
#include "GenericUART.hpp"

class WirelessUART : public GenericUART{

	public:
	WirelessUART():
			GenericUART(GENERIC_UART_TYPE_USART3_PB10_PB11, 115200){
		}


	private:

};
