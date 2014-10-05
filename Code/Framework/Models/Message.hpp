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
					message_type = msg_type;
					set_float(value);
		}

		Message(MessageType msg_type, uint8_t data0 = 0,
									  uint8_t data1 = 0,
									  uint8_t data2 = 0,
									  uint8_t data3 = 0,
									  uint8_t data4 = 0,
									  uint8_t data5 = 0){

			message_type  = msg_type;
			data[0] = data0;
			data[1] = data1;
			data[2] = data2;
			data[3] = data3;
			data[4] = data4;
			data[5] = data5;
		}

		Message(MessageType msg_type, void* value){
					message_type = msg_type;
					set_pointer(value);
		}

	   /********************************************
		* 			GETTERS and SETTERS				*
		********************************************/
		void set_float(float float_value){
			*(float*)data = float_value;
		}

		float get_float(){
			return *(float*)data;
		}

		void set_integer32(uint32_t int_value){
			*(uint32_t*)data = int_value;
		}

		uint32_t get_integer32(){
			return *(uint32_t*)data;
		}

		void set_byte_array(uint8_t* bytearray){
			for(uint8_t i=0; i<6; i++){
				data[i] = bytearray[i];
			}
		}

		void get_byte_array(uint8_t* bytearray){
			for(uint8_t i=0; i<6; i++){
				bytearray[i] = data[i];
			}
		}

		void set_pointer(void* ptr_value){
			ptr = ptr_value;
		}

		void* get_pointer(){
			return ptr;
		}


	   /********************************************
		* 			  MESSAGE DATA					*
		********************************************/
		Messenger* 	 	sender;
		Messenger* 	 	receiver;
		MessageType	 	message_type;
		uint8_t 		data[6];
		void*			ptr;
	private:

};
