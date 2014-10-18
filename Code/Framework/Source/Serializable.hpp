#pragma once

#include <stdio.h>
#include <string.h>


class Serializable_I{
public:
	//Returns the number of bytes serialized:
	virtual uint32_t serialize(uint8_t*) = 0;
	virtual void deserialize(uint8_t*) = 0;

};
