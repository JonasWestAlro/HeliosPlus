#pragma once

#include <stdio.h>
#include <string.h>

class PrintHelper{
public:
	/*static uint16_t print(char* buffer, uint32_t value){

		return 0;
	}*/

	static uint16_t print(char* buffer, int32_t value){

		return 0;
	}

	static uint16_t print(char* buffer, float value){
	    uint8_t i = 0;
	    uint8_t len = snprintf(buffer, 10, "%f", value);

	    return len;
	}

	static uint16_t print(char* buffer, uint8_t value){

		return 0;
	}

	static uint16_t print(char* buffer, int8_t value){

		return 0;
	}

	static uint16_t print(char* buffer, uint16_t value){

		return 0;
	}

	static uint16_t print(char* buffer, int16_t value){

		return 0;
	}
};
