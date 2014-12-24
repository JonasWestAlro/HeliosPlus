#include "Global.hpp"

namespace Globals{
	//This is the pointer to the filesystem, in which the globals
	//should be saved. It has to be created in the main, after timers and eeprom
	//has been initialized.
	FileSystem* filesystem;

	static uint16_t no_globals;
	static GlobalAbstract* global_table[FRAMEWORK_MAX_GLOBALS];

	void init(FileSystem* filesystem_){
		filesystem = filesystem_;

		for(uint8_t i=0; i<Globals::get_no_globals(); i++){
			Globals::get_globals_table()[i]->set_filesystem(filesystem);
		}

		Global<float>::load_all();
	}

	GlobalAbstract** get_globals_table(){
		return global_table;
	}

	uint8_t add_global(GlobalAbstract* obj){
		global_table[no_globals++] = obj;

		return no_globals-1;
	}

	uint16_t get_no_globals(){
		return no_globals;
	}
}
