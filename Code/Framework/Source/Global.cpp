#include "Global.hpp"

//!todo Put these in a namespace
static uint16_t no_globals;
static GlobalAbstract* global_table[FRAMEWORK_MAX_GLOBALS];

void add_global(GlobalAbstract* obj){
	global_table[no_globals++] = obj;
}

uint16_t get_no_globals(){
	return no_globals;
}
