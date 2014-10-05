#pragma once

#include "Framework.hpp"
#include "Communication.hpp"
#include "ControlInput.hpp"
#include "FlightControl.hpp"
#include "FlightDynamics.hpp"
#include "FlightNavigation.hpp"


class SystemStatus : public ApplicationModule{
	public:
		// We need to have eeprom last if we want default size on it
		SystemStatus(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size,
					Communication* communication, ControlInput* controlinput, FlightControl* flightcontrol,
					FlightDynamics* flightdynamics, FlightNavigation* flightnavigation);

	protected:
		void task(void);

	private:
		void handle_message(Message& msg);

		Communication* 		m_Communication;
		ControlInput*  		m_ControlInput;
		FlightControl* 		m_FlightControl;
		FlightDynamics* 	m_FlightDynamics;
		FlightNavigation* 	m_FlightNavigation;

};

