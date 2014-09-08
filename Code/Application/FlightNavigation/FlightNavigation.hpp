#pragma once

#include "Framework.hpp"

class FlightNavigation : public ApplicationModule {
	public:
		FlightNavigation(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);
		APP_Attitude_I attitude;

	protected:
		void run(void);

	private:
		uint8_t dummy;
};

