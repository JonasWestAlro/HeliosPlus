#pragma once

#include "Framework.hpp"

class HAL_Eeprom_I : public HAL_Interface {
	public:
	virtual uint8_t write(uint16_t, uint8_t, uint8_t*) = 0;
	virtual uint8_t read(uint16_t, uint8_t, uint8_t*) = 0;
};
