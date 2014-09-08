#pragma once

#include "Framework.hpp"

class ApplicationModule : Task {
	public:
		ApplicationModule(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0):
			Task(name, stackSize, priority),
			messenger(),
			eeprom(name, eeprom_size){

		}


	protected:
		EEPROM eeprom;
		Messenger messenger;

	private:


};

