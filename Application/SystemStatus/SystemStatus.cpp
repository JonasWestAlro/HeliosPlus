#include "SystemStatus.hpp"

void SystemStatus::init(){
	messenger.subscribe(REQUEST_SHIFT_OF_CONTROL);
}

void SystemStatus::SystemStatus::run(void)
{
	init();
	Message msg;

	while(1){
		while(messenger.try_receive(&msg)){
			handle_message(msg);
		}



		vTaskDelay(300);
	}
}

void SystemStatus::handle_message(Message& msg){
	int i = 0;

	switch(msg.message_type){
		case REQUEST_SHIFT_OF_CONTROL:
			//We'll acknowledge the shift:

			messenger.send_to(msg.sender, Message(SHIFT_OF_CONTROL_ACK, (uint8_t)ACK));
			messenger.respond(msg,  Message(SHIFT_OF_CONTROL_ACK, (uint8_t)ACK));

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

