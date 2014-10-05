#include "SystemStatus.hpp"


SystemStatus::SystemStatus(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
						   Communication* communication, ControlInput* controlinput, FlightControl* flightcontrol,
						   FlightDynamics* flightdynamics, FlightNavigation* flightnavigation)
: ApplicationModule(name, stackSize, priority, eeprom_size),
  m_Communication(communication),
  m_ControlInput(controlinput),
  m_FlightControl(flightcontrol),
  m_FlightDynamics(flightdynamics),
  m_FlightNavigation(flightnavigation)
{
	// Initializer code goes here
	messenger.subscribe(REQUEST_SHIFT_OF_CONTROL);
}

void SystemStatus::task(void)
{

}

void SystemStatus::handle_message(Message& msg){
	int i = 0;

	switch(msg.message_type){
		case REQUEST_SHIFT_OF_CONTROL:
			//We'll acknowledge the shift:

			messenger.send_to(msg.sender, Message(SHIFT_OF_CONTROL_ACK, (uint8_t)ACK));
			messenger.respond(msg, Message(SHIFT_OF_CONTROL_ACK, (uint8_t)ACK));

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

