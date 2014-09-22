#pragma once

#include "Framework.hpp"
#include "GenericUART.hpp"

class UART : public GenericUART{

	public:
	UART():
			GenericUART(GENERIC_UART_TYPE_UART4_PC10_PC11, 115200){
		}


	private:

};
