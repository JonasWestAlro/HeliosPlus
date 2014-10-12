#pragma once

#include "Framework.hpp"
#include "MessageTypes.hpp"


class Message{
	public:
	   /********************************************
		* 				CONSTRUCTORS			   *
		********************************************/

		Message(){
		}

		Message(MessageType msg_type, float value){
			type = msg_type;
			set_float(value);
		}

		Message(MessageType msg_type, uint8_t data0 = 0,
									  uint8_t data1 = 0,
									  uint8_t data2 = 0,
									  uint8_t data3 = 0,
									  uint8_t data4 = 0,
									  uint8_t data5 = 0)
		{
			type  = msg_type;
			mydata.data[0] = data0;
			mydata.data[1] = data1;
			mydata.data[2] = data2;
			mydata.data[3] = data3;
			mydata.data[4] = data4;
			mydata.data[5] = data5;
		}

		Message(MessageType msg_type, void* value){
			type = msg_type;
			set_pointer(value);
		}

	   /********************************************
		* 			GETTERS and SETTERS				*
		********************************************/
		void set_float(float float_value){
			mydata.f = float_value;
		}

		float get_float(){
			return mydata.f;
		}

		void set_integer32(uint32_t int_value){
			mydata.i = int_value;
		}

		uint32_t get_integer32(){
			return mydata.i;
		}

		void set_byte_array(uint8_t* bytearray){
			for(uint8_t i=0; i<6; i++){
				mydata.data[i] = bytearray[i];
			}
		}

		void get_byte_array(uint8_t* bytearray){
			for(uint8_t i=0; i<6; i++){
				bytearray[i] = mydata.data[i];
			}
		}

		void set_byte(uint8_t index, uint8_t data){
			mydata.data[index] = data;
		}

		uint8_t get_byte(uint8_t index){
			return mydata.data[index];
		}

		void set_pointer(void* ptr_value){
			ptr = ptr_value;
		}

		void* get_pointer(){
			return ptr;
		}

		uint8_t get_enum(){
			return enum_data;
		}

		void set_enum(uint8_t enum_data_){
			enum_data = enum_data_;
		}

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
