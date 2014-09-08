#include "FlightNavigation.hpp"


void FlightNavigation::init(){}

void FlightNavigation::run(void)
{
	while(1)
	{
		attitude.Pitch = 10;
		attitude.receive();

		vTaskDelay(300);
	}
}
