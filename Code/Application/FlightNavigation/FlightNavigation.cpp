#include "FlightNavigation.hpp"


FlightNavigation::FlightNavigation(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	// Initializer code goes here
}

void FlightNavigation::task(void)
{

		attitude.pitch = 10;
		attitude.receive();


}
