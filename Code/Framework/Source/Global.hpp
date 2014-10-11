#pragma once

#include "Framework.hpp"

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



//!todo Put these in a namespace
static uint16_t no_globals;
static GlobalAbstract* global_table[FRAMEWORK_MAX_GLOBALS];

static void add_global(GlobalAbstract* obj){
	global_table[no_globals++] = obj;
}

static uint16_t get_no_globals(){
	return no_globals;
}
