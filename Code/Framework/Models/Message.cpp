#include "Message.hpp"


Message::Message(){

}

//! Explicit constructor
Message::Message(MessageType msg_type, float value){
	type = msg_type;
	set_float(value);
}

//! Explicit constructor
Message::Message(MessageType msg_type, void* value){
	type = msg_type;
	set_pointer(value);
}

//! Explicit constructor
Message::Message(MessageType msg_type, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5){
	type  = msg_type;
	mydata.data[0] = data0;
	mydata.data[1] = data1;
	mydata.data[2] = data2;
	mydata.data[3] = data3;
	mydata.data[4] = data4;
	mydata.data[5] = data5;
}


void Message::set_float(float float_value){
	mydata.f = float_value;
}

float Message::get_float(){
	return mydata.f;
}

void Message::set_integer32(uint32_t int_value){
	mydata.i = int_value;
}

uint32_t Message::get_integer32(){
	return mydata.i;
}

void Message::set_byte_array(uint8_t* bytearray){
	for(uint8_t i=0; i<6; i++){
		mydata.data[i] = bytearray[i];
	}
}

void Message::get_byte_array(uint8_t* bytearray){
	for(uint8_t i=0; i<6; i++){
		bytearray[i] = mydata.data[i];
	}
}

void Message::set_byte(uint8_t index, uint8_t data){
	mydata.data[index] = data;
}

uint8_t Message::get_byte(uint8_t index){
	return mydata.data[index];
}

void Message::set_pointer(void* ptr_value){
	ptr = ptr_value;
}

void* Message::get_pointer(){
	return ptr;
}

uint8_t Message::get_enum(){
	return enum_data;
}

void Message::set_enum(uint8_t enum_data_){
	enum_data = enum_data_;
}
