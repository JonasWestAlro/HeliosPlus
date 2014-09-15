#pragma once

#include "Framework.hpp"

typedef enum{
	LED_ON,
	LED_OFF
}LED_state;

typedef enum{
	STATUS_LED_RED,   //red if armed - green if not - yellow if calibrating
	STATUS_LED_GREEN,
	DEBUG_RED,
	DEBUG_YELLOW,
	DEBUG_BLUE1,
	DEBUG_BLUE2
}HeliosLED;

class HAL_HeliosLED_I : public HAL_Interface {
	public:
	virtual void set_LED(HeliosLED_t, LED_state_t) = 0;
	virtual LED_state_t get_LED(HeliosLED_t) = 0;
};
