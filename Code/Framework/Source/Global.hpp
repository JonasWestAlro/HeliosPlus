#pragma once
/**************************************************************************//**
 * @file     Global.hpp
 * @brief
 * @version  V1.0
 * @date     13. October 2014
 *
 * @note
 * This file is a part of the framework develop for Helios Flightcontroller
 *
 ******************************************************************************/


#include "Mutex.hpp"
#include <stdint.h>
#include "PrintHelper.hpp"
#include "SerializeHelper.hpp"
#include "Storable.hpp"
#include "Filesystem.hpp"

//!todo Make a Framework Configuration file just like in freeRTOS and put this define there
#define FRAMEWORK_MAX_GLOBALS 		30



class GlobalAbstract;

namespace Globals{
	extern FileSystem filesystem;
	void init();
	uint8_t add_global(GlobalAbstract* obj);
	uint16_t get_no_globals();
	GlobalAbstract** get_globals_table();
}

class GlobalAbstract : public Storable {
public:
	GlobalAbstract(const char* name_):
		name(name_),
		Storable(&Globals::filesystem, const_cast<char*>(name_)){
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

	static void save_all(){
		uint8_t i = 0;
		uint8_t buffer[8];

		for(i=0; i<Globals::get_no_globals(); i++){
			Globals::get_globals_table()[i]->save(buffer);
		}
	}

	static void load_all(){
		uint8_t i = 0;
		uint8_t buffer[8];

		for(i=0; i<Globals::get_no_globals(); i++){
			if(Globals::get_globals_table()[i]->load(buffer) == FILE_CREATED){
				Globals::get_globals_table()[i]->save(buffer);
			}
		}
	}

	//Storable interface:
	virtual uint32_t serialize(uint8_t* buffer){
		SerializeHelper::serialize(buffer, data);
	};

	virtual void deserialize(uint8_t* buffer){
		SerializeHelper::deserialize(buffer, data);
	};

	virtual uint32_t get_size(){return sizeof(T);}


private:
	T data;
	Mutex mutex;
};


