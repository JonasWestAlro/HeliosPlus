#include "FlightControl.hpp"

FlightControl::FlightControl(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	// Initializer code goes here
}

void FlightControl::task(void)
{

	attitude.pitch = 10;
	attitude.receive();

}
