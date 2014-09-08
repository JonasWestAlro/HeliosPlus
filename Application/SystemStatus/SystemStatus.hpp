#pragma once

#include "Framework.hpp"

class SystemStatus : public ApplicationModule{
	using ApplicationModule::ApplicationModule;

	public:

	protected:
		void init(void);
		void run(void);

	private:
		uint8_t dummy;

		void handle_message(Message& msg);

};

