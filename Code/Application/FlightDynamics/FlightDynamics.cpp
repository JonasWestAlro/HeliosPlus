#include "FlightDynamics.hpp"


FlightDynamics::FlightDynamics(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	// Initializer code goes here

	messenger.subscribe(CALIBRATE_GYROSCOPE);
	messenger.subscribe(CALIBRATE_ACCELEROMETER);

	eeprom.load(&eeprom_structure, sizeof(EEPROM_Structure));
}

void FlightDynamics::run(void){
	Message msg;

	while(1){
		while(messenger.try_receive(&msg)){
			handle_message(msg);
		}

		accelerometer->is_alive();

		vTaskDelay(300);
	}
}

void FlightDynamics::handle_message(Message& msg){
	int i = 0;

	switch(msg.message_type){
		case CALIBRATE_ACCELEROMETER:
			i++;
			i++;
			break;
		case CALIBRATE_GYROSCOPE:
			i++;
			i++;
			break;
		default:
			break;
	}

	//Respond to the sender:


	return;
}
