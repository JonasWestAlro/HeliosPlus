#pragma once

#include "Framework.hpp"



class ControlInput : public ApplicationModule{
	using ApplicationModule::ApplicationModule;

	public:


		APP_Attitude_I attitude;
	protected:
		void init(void);
		void run(void);

	private:

		void handle_internal_message(void);
		void handle_switch(float state);
		void handle_clicker(float state);
		void check_arm(void);
		void switch_arm_state(void);
		void request_control(uint8_t Request);
		void calibrate_start(void);
		void calibrate_stop(void);
		void update_status(void);
		void report_status(void);
};
