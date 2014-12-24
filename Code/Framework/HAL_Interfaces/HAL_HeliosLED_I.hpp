#pragma once

#include "HAL_Interface.hpp"

typedef enum{
	LED_ON,
	LED_OFF
}LED_STATE;

typedef enum{
	STATUS_LED_RED,   //red if armed - green if not - yellow if calibrating
	STATUS_LED_GREEN,
	DEBUG_RED,
	DEBUG_YELLOW,
	DEBUG_BLUE1,
	DEBUG_BLUE2
}HELIOS_LED;

class HAL_HeliosLED_I : public HAL_Interface {
	public:
	virtual void set_LED(HELIOS_LED, LED_STATE) = 0;
	virtual LED_STATE get_LED(HELIOS_LED) = 0;

	void all_on(){
		set_LED(STATUS_LED_RED, LED_ON);
	}

};
