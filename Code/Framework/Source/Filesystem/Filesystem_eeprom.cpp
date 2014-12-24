#include "Filesystem_eeprom.h"

#include "Filesystem.hpp"
#include "Timing.hpp"
#include "Framework.hpp"

uint32_t eeprom_errors = 0;

uint32_t get_eeprom_errors(){
	return eeprom_errors;
}

void eeprom_write(void* offset, void* data, uint32_t size){
	uint32_t uint16_offset = (uint32_t)offset;

	Time.delay_ms(5);

	portENTER_CRITICAL();
	uint8_t result = GlobalFileSystemHandler::get_instance().get_driver()->write(uint16_offset, size, (uint8_t*)data);
	portEXIT_CRITICAL();

	if(!result){
		eeprom_errors++;
	}
}

void eeprom_read(void* buffer, void* offset, uint32_t size){
	uint32_t uint16_offset = (uint32_t)offset;

	Time.delay_ms(5);
	portENTER_CRITICAL();
	uint8_t result = GlobalFileSystemHandler::get_instance().get_driver()->read(uint16_offset, size, (uint8_t*)buffer);
	portEXIT_CRITICAL();

	if(!result){
		eeprom_errors++;
	}
}
