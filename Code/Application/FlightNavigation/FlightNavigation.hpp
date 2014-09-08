#pragma once

#include "Framework.hpp"

class FlightNavigation : public ApplicationModule {
	public:
		using ApplicationModule::ApplicationModule; // Use base class constructor
		APP_Attitude_I attitude;
	protected:
		void init(void);
		void run(void);

	private:
		uint8_t dummy;
};

