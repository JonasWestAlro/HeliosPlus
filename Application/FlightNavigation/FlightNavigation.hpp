#pragma once

#include "Framework.hpp"

class FlightNavigation : public ApplicationModule{
	using ApplicationModule::ApplicationModule;
	public:
		APP_Attitude_I attitude;
	protected:
		void init(void);
		void run(void);

	private:
		uint8_t dummy;
};

