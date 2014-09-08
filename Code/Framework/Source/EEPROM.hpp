#pragma once

#include "Global.hpp"

class EEPROM{
public:
	EEPROM(const char* id_, uint32_t size_):
		id(id_){

		offset = counter;
		counter += size_;
		size = size_;

		EEPROM_table[table_counter++] = this;
	}

	bool save(void* data, uint32_t size){
		//TODO-JWA: Implement this function..
		return true;
	}

	bool load(void* data, uint32_t size){
		//TODO-JWA: Implement this function..
		return true;
	}

	//HAL_EEPROM_I* eeprom_driver;
private:
	static uint32_t counter;
	static EEPROM* 	EEPROM_table[10];
	static uint8_t 	table_counter;

	uint32_t 	offset;
	uint32_t 	size;
	const char* id;
};


