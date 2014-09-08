#pragma once

#include "EEPROM.hpp"
#include "Messenger.hpp"
#include "Task.hpp"

class ApplicationModule : Task {
	public:
		ApplicationModule(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0)
		: Task(name, stackSize, priority),
		  eeprom(name, eeprom_size),
		  messenger()
		{}


	protected:
		EEPROM eeprom;
		Messenger messenger;


	private:


};

