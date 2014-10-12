#pragma once

#include "Mutex.hpp"
#include <stdint.h>

//!todo Make a Framework Configuration file just like in freeRTOS and put this define there
#define FRAMEWORK_MAX_GLOBALS 		30

class GlobalAbstract{
public:
	virtual ~GlobalAbstract(){}
};

template<typename T>
class Global : public GlobalAbstract{
public:
	Global(T value, const char* name_):
		name(name_){
		data = value;
		add_global(this);
	}

	virtual ~Global(){}

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

	const char* get_id(){
		return name;
	}

	bool save(){
		//Use portable EEPROM driver to save..
		return true;
	}

	bool load(){
		//Use portable EEPROM driver to load..
		return true;
	}
private:
	T data;
	Mutex mutex;
	const char* name;
};

void add_global(GlobalAbstract* obj);
uint16_t get_no_globals();

