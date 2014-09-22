#pragma once

#include "Framework.hpp"
#include "GenericUART.hpp"

class MasterUART : public GenericUART{

	public:
		MasterUART():
			GenericUART(GENERIC_UART_TYPE_USART2_PD5_PD6, 230400){
		}


	private:

};
