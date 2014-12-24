#include "FrameworkHooks.hpp"

void corrupted_filesystem_hook(){
	uint8_t c;

	Debug.put_and_transmit("\n\n\rFILE SYSTEM IS CORRUPTED!..\n\push y to create new filesystem..");

	while(1){
		if(Debug.data_available()){
			Debug.receive(&c);
			if(c == 'y'){
				break;
			}else{
				Debug.put_and_transmit("\n\n\rFILE SYSTEM IS CORRUPTED!..\n\r -push y to create new filesystem..");
			}
		}
	}
}
