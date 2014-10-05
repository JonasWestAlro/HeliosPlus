#include "Application/ControlInput/ControlInput.hpp"


ControlInput::ControlInput(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	// Init here
}

void ControlInput::task(void)
{

		attitude.pitch = 100;
		attitude.publish();


}

/****************************************************************
 * 						PRIVATE FUNCTIONS						*
 ****************************************************************/
void ControlInput::handle_internal_message(void)
{

}

void ControlInput::update_status(void)
{

}

void ControlInput::handle_switch(float state)
{

}

void ControlInput::handle_clicker(float state)
{

}

void ControlInput::check_arm(void)
{

}

void ControlInput::switch_arm_state(void)
{

}

void ControlInput::request_control(uint8_t Request)
{

}

void ControlInput::calibrate_start(void)
{

}

void ControlInput::calibrate_stop(void)
{

}

void ControlInput::report_status(void)
{

}
