#pragma once

#include "Framework.hpp"

class FlightControl : public ApplicationModule{
	public:
		FlightControl(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);
		APP_Attitude_I attitude;

	protected:
		void task(void);
		void handle_message(Message& msg){}
	private:

};

