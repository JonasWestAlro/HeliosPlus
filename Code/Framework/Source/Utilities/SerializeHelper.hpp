#pragma once


class SerializeHelper{
public:

	/****************************************************
	 * 				SERIALIZATION HELPERS				*
	 ****************************************************/
	static uint8_t serialize(uint8_t* buffer, float value){
		serialize_float(buffer, value);
	}

	static uint8_t serialize_uint32(uint8_t* buffer, uint32_t value){
		memcpy(buffer, &value, sizeof(uint32_t));
		return sizeof(uint32_t);
	}

	static uint8_t serialize_float(uint8_t* buffer, float value){
		memcpy(buffer, &value, sizeof(float));
		return sizeof(float);
	}

	static uint8_t serialize_char(char* buffer, char value){
		memcpy(buffer, &value, sizeof(char));
		return sizeof(char);
	}

	static uint8_t serialize_uint8(uint8_t* buffer, uint8_t value){
		memcpy(buffer, &value, sizeof(uint8_t));
		return sizeof(char);
	}

	static uint8_t serialize_int32(uint8_t* buffer, int32_t value){
		memcpy(buffer, &value, sizeof(int32_t));
		return sizeof(int32_t);
	}

	static uint8_t serialize_string(uint8_t* buffer, const char* string_value){
		//First we'll use 1 byte to describe the lenght of the string:
		uint8_t lenght = strlen(string_value);
		buffer[0] = lenght;

		//Then we'll copy the string to the buffer:
		memcpy(buffer+1, string_value, lenght);

		return lenght+1;
	}

	/****************************************************
	 * 				DESERIALIZATION HELPERS				*
	 ****************************************************/
	static uint8_t deserialize(uint8_t* buffer, float& value){
		deserialize_float(buffer, value);
	}

	static uint8_t deserialize_float(uint8_t* buffer, float& value){
		memcpy(&value, buffer, sizeof(float));

		return sizeof(float);
	}


};
