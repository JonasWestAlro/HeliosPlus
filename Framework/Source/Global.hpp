#pragma once

#include "Framework.hpp"

template<typename T>
class Global{
public:
	Global(T value, const char* name_):
		name(name_){
		data = value;

		//TODO-JWA: This won't compile for some reason..
		//global_table[no_globals] = this;
		no_globals++;
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

	const char* get_id(){
		return name;
	}

	bool save(){
		//Use portable EEPROM driver to save..
	}

	bool load(){
		//Use portable EEPROM driver to load..
	}

	static uint16_t get_no_globals(){
		return no_globals;
	}
private:
	T data;
	Mutex mutex;
	const char* name;

	static uint16_t no_globals;
	static Global* global_table[FRAMEWORK_MAX_GLOBALS];
};
