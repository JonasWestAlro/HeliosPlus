#pragma once

#include "Framework.hpp"
#include "GenericUART.hpp"

class GPS_UART : public GenericUART{

	public:
		GPS_UART():
			GenericUART(GENERIC_UART_TYPE_UART5_PC12_PD2, 9600){
		}


	private:

};
