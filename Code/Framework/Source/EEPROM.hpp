#pragma once

#include "Global.hpp"
#include "Serializable.hpp"
#include "HAL_Eeprom_I.hpp"

class EepromSpace;

static const uint16_t EEPROM_TABLE_MAX_SIZE = 400;

//The purpose of the EepromTable is to manage the different EEPROMs (in the application modules).
class EepromHandler {
public:
	EepromHandler(){

	}

	EepromHandler(HAL_Eeprom_I* driver){
		eeprom = driver;
	}

	uint32_t add_eeprom_object(Serializable_I* obj, uint16_t size){
		uint32_t result_offset = offset_counter;

		offset_counter += size;
		EEPROM_table[table_counter++] = obj;

		return result_offset;
	}

	uint32_t serialize(uint8_t* buffer){
		uint8_t* buffer_start = buffer;

		//We'll first serialize the number of table entries:
		buffer += Serialize::serialize_uint8(buffer, table_counter);

		//Now serialize each table entry:
		for(uint8_t i = 0; i<table_counter; i++){
			buffer += EEPROM_table[i]->serialize(buffer);
		}

		return (uint32_t)(buffer-buffer_start);
	}

	void deserialize(uint8_t* buffer){

	}

	bool synchronize(){
		//In this function we'll check if the saved EEPROM table is the same as the current.
		//If it isn't we'll delete everything and start over.

		//Compute the current eeprom_table:
		uint8_t eeprom_table_buffer[EEPROM_TABLE_MAX_SIZE] = {0};
		uint32_t eeprom_table_size = serialize(eeprom_table_buffer);

		//Load the stored table from EEPROM:
		uint8_t eeprom_table_stored_buffer[EEPROM_TABLE_MAX_SIZE] = {0};
		eeprom->read(0, EEPROM_TABLE_MAX_SIZE, eeprom_table_stored_buffer);

		//Compare:
		if(!memcmp(eeprom_table_stored_buffer,eeprom_table_buffer,EEPROM_TABLE_MAX_SIZE)){
			//Success the two tables are identical, and we can continue.
			return true;
		}else{
			//Something has been changed in the program storage structure,
			//so we'll erase everything and start over..
			//TODO-JWA: We should probably have some upgrade policy??
			//			so we can be backward compatible when upgrading to a new version.

			eeprom->erase_all();
			if(!eeprom->write(0, eeprom_table_size, eeprom_table_buffer)){
				//Something went wrong!!
				//TODO-JWA: We should handle it properly - it could have fatal consequences..
			}

			return false;
		}
	}

	void set_eeprom(HAL_Eeprom_I* eeprom_driver){
		eeprom = eeprom_driver;
	}

	HAL_Eeprom_I* get_driver(){
		return eeprom;
	}

private:
	HAL_Eeprom_I* eeprom;

	Serializable_I*  EEPROM_table[10];
	uint32_t 		 offset_counter = 0;
	uint8_t  		 table_counter = 0;
};




class EepromSpace : public Serializable_I {
public:
	EepromSpace(const char* id_, uint32_t size_, EepromHandler* handler_):
		id(id_){

		handler = handler_;
		size 	= size_;
		offset 	= handler->add_eeprom_object(this, size);
	}

	bool save(void* data, uint32_t size){
		//TODO-JWA: Implement this function..

		 handler->get_driver()->write(0,0,0);

		return true;
	}

	bool load(void* data, uint32_t size){
		//TODO-JWA: Implement this function..

		handler->get_driver()->read(0,0,0);

		return true;
	}

	uint16_t get_size(){return size;}

	//These are for the Serializable_I interface:---------------
	virtual uint32_t serialize(uint8_t* buffer){
		uint8_t* buffer_start = buffer;

		buffer += Serialize::serialize_uint32(buffer, offset);
		buffer += Serialize::serialize_uint32(buffer, size);
		buffer += Serialize::serialize_string(buffer, id);

		return (uint32_t)(buffer-buffer_start);
	}

	virtual void deserialize(uint8_t* buffer){

	}
	//--------------------

private:
	EepromHandler* 	handler;
	uint32_t 		offset;
	uint32_t 		size;
	const char* 	id;
};


