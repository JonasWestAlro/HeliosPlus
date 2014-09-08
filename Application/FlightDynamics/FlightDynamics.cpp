#include "FlightDynamics.hpp"

void FlightDynamics::init(void){
	messenger.subscribe(CALIBRATE_GYROSCOPE);
	messenger.subscribe(CALIBRATE_ACCELEROMETER);

	eeprom.load(&eeprom_structure, sizeof(EEPROM_Structure));
}

void FlightDynamics::run(void){
	init();
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
