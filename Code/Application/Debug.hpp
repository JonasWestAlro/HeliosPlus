#pragma once

#include "HAL_Communication_I.hpp"
#include <initializer_list>
class DebugClass{
public:

	void set_driver(HAL_Communication_I* com){communication = com;};

	void send_and_transmit_floats(std::initializer_list<float> list){
		 for( auto elem : list ){
			 send_number(elem);
			 send(',');
		}

		put("\n\r");
		transmit();
	}

	void send_and_transmit_uint32(std::initializer_list<uint32_t> list){
		 for( auto elem : list ){
			 send_number(elem);
			 send(',');
		}

		put("\n\r");
		transmit();
	}

	void put_and_transmit(const char* debugstring){
		if(communication != 0){
			communication->put(debugstring);
			while(!communication->transmit());
		}
	}

	void put(const char* debugstring){
		if(communication != 0){
			communication->put(debugstring);
		}
	}

	void put(const char* debugstring, uint16_t size){
		if(communication != 0){
			communication->put(debugstring, size);
		}
	}

	void transmit(){
		if(communication != 0){
			communication->transmit();
		}
	}

	void send(uint8_t c){
		if (communication != 0) {
			communication->send(c);
		}
	}

	void send_and_transmit(uint8_t c){
		if (communication != 0) {
			communication->send(c);
			while(!communication->transmit());
		}
	}

	uint16_t data_available(void){
		if(communication != 0){
			return communication->data_available();
		}
		return false;
	};

	bool receive(uint8_t* c){
		if(communication != 0){
			if(communication->data_available()){
				return communication->receive(c);
			}
		}
		return false;
	}

	bool send_number(uint32_t c){return communication->send_number(c);};
	bool send_number(float c){return communication->send_number(c);};


private:
	HAL_Communication_I* communication = 0;

};

extern DebugClass Debug;
