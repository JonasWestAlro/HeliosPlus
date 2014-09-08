#include "FlightControl.hpp"

void FlightControl::init(){

}

void FlightControl::run(void)
{
	while(1)
	{
		attitude.Pitch = 10;
		attitude.receive();


		vTaskDelay(300);
	}
}
