#pragma once
/**************************************************************************//**
 * @file     Message.hpp
 * @brief
 * @version  V1.0
 * @date     13. October 2014
 *
 * @note
 * This file is a part of the framework develop for Helios Flightcontroller
 *
 ******************************************************************************/


//! Forward declaration
class Messenger;

#include "Messenger.hpp"
#include "MessageTypes.hpp"
#include <stdint.h>


class Message{
	public:
		//! Default constructor
		Message();

		//! Explicit constructor
		Message(MessageType msg_type, float value);
		Message(MessageType msg_type, void* value);
		Message(MessageType msg_type, uint8_t data0 = 0, uint8_t data1 = 0, uint8_t data2 = 0, uint8_t data3 = 0, uint8_t data4 = 0, uint8_t data5 = 0);

		void set_float(float float_value);
		float get_float();

		void set_integer32(uint32_t int_value);
		uint32_t get_integer32();

		void set_byte_array(uint8_t* bytearray);
		void get_byte_array(uint8_t* bytearray);

		void set_byte(uint8_t index, uint8_t data);
		uint8_t get_byte(uint8_t index);

		void set_pointer(void* ptr_value);
		void* get_pointer();

		uint8_t get_enum();
		void set_enum(uint8_t enum_data_);


	   /********************************************
		* 			  MESSAGE DATA					*
		********************************************/
		Messenger* 	 	sender;
		Messenger* 	 	receiver;
		MessageType	 	type;


	private:
		union mydata_t{
			uint8_t 	data[6];
			float		f;
			uint32_t	i;
		}mydata;
		uint8_t			enum_data;
		void*			ptr;
};
