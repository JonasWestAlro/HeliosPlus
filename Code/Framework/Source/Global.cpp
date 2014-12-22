#include "Global.hpp"

namespace Globals{
	//FileSystem filesystem("/GLOBALS", 5000);

	static uint16_t no_globals;
	static GlobalAbstract* global_table[FRAMEWORK_MAX_GLOBALS];

	void init(){
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
