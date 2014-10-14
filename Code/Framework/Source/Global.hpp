#pragma once

#include "Mutex.hpp"
#include <stdint.h>
#include "PrintHelper.hpp"

//!todo Make a Framework Configuration file just like in freeRTOS and put this define there
#define FRAMEWORK_MAX_GLOBALS 		30

class GlobalAbstract;

namespace Globals{
	uint8_t add_global(GlobalAbstract* obj);
	uint16_t get_no_globals();
	GlobalAbstract** get_globals_table();
}

class GlobalAbstract{
public:
	GlobalAbstract(const char* name_):
		name(name_){
		index_in_global_table = Globals::add_global(this);
	}

	virtual uint16_t print(char*) = 0;

	virtual ~GlobalAbstract(){}

	const char* get_id(){
		return name;
	}

private:
	const char* name;
	uint8_t index_in_global_table = 0;
};

template<typename T>
class Global : public GlobalAbstract{
public:
	Global(T value, const char* name_):
		GlobalAbstract(name_){
		data = value;
	}

	virtual ~Global(){}

	virtual uint16_t print(char* buffer){
		return PrintHelper::print(buffer, data);
	}

	void set(T value){
		mutex.take();
			data = value;
		mutex.release();
	}

	T get(){
		T temp;
		mutex.take();
			temp = data;
		mutex.release();
		return temp;
	}

	bool save(){
		//Use portable EEPROM driver to save..
		return true;
	}

	bool load(){
		//Use portable EEPROM driver to load..
		return true;
	}

	static void save_all(){

	}

	static void load_all(){
		//We have to check if the global table saved in the eeprom is
		//is same as the current local one, becuase if it ain't it means
		//that there has been introduced new globals or similar, and we don't
		//know how
	}

private:
	T data;
	Mutex mutex;
};


