#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "eefs_filesys.h"
#include "HAL_Eeprom_I.hpp"
#include "FrameworkSettings.h"

class FileSystem;

class GlobalFileSystemHandler {
public:
	static GlobalFileSystemHandler& get_instance(void){
		static GlobalFileSystemHandler handler;
		return handler;
	}

	uint32_t add_new_filesystem(FileSystem* filesystem, uint16_t size){
		uint32_t result_offset = offset_counter;

		offset_counter += size;
		filesystem_table[table_counter++] = filesystem;

		return result_offset;
	}

	uint8_t get_no_filesystems(){
		return table_counter;
	}

	FileSystem** get_filesystem_table(){
		return filesystem_table;
	}

	void set_driver(HAL_Eeprom_I* driver){storage_device = driver;}
	HAL_Eeprom_I* get_driver(){return storage_device;}

	bool get_generate_new_filesystem_flag(){return generate_new_filesystem;}
	void set_generate_new_filesystem_flag(bool f){generate_new_filesystem = f;}

private:
	GlobalFileSystemHandler(){}

	HAL_Eeprom_I* 	storage_device;

	FileSystem*  	filesystem_table[10];
	uint32_t 		offset_counter = 0;
	uint8_t  		table_counter = 0;

	bool generate_new_filesystem = 0;
};

extern uint8_t eeprom_buffer[10000];

class FileSystem{
public:
	FileSystem(const char* volume, uint32_t size_):
		volume_name(volume){

		int32 initialize_result = 0;
		int8_t volume_name_found = 0;

		size = size_;

		//Find offset from GlobalFilesystemHandler:
		//(uint32_t)&eeprom_buffer +
		name_offset = GlobalFileSystemHandler::get_instance().add_new_filesystem(this, size);
		header_offset = name_offset + EEFS_MAX_DEVICENAME_SIZE;

		//Check if the given volume name is found at that offset:
		if(!volume_exist(volume, name_offset)){
			//No volume by that name exist at the given offset.
			//Are we allowed to create a new filesystem?
			bool generate_new_filesystem = GlobalFileSystemHandler::get_instance().get_generate_new_filesystem_flag();
			if(!generate_new_filesystem){
				//No we are not. Then we'll have to call the corrupted filesystem hook:
				CORRUPTED_FILESYSTEM_HOOK;

				//Now it's OK to create the new filesystem!
				GlobalFileSystemHandler::get_instance().set_generate_new_filesystem_flag(1);
			}

			Debug.put("\n\rCreating new volume: ");
			Debug.put(volume);
			create_filesystem(volume, name_offset, size);
		}

		//Initialize filesystem:
		initialize_result = EEFS_InitFS(const_cast<char*>(volume_name), header_offset);

		//If the filesystem doesn't exist for some reason.
		if(initialize_result == EEFS_NO_SUCH_DEVICE){
				//Something is very wrong!!
				while(1);
		}

		eefs_device = EEFS_GetDevicePtr(initialize_result);
		initialize_result = EEFS_Mount(const_cast<char*>(volume_name), const_cast<char*>(volume_name));

		Debug.transmit();
	}

	uint8_t create_filesystem(const char* volume_name, uint32_t offset, uint16_t size){
		//Burn the name of the volume down first:
		EEFS_LIB_EEPROM_WRITE((void*)offset, (void*)volume_name, EEFS_MAX_DEVICENAME_SIZE);

		//update offset:
		offset += EEFS_MAX_DEVICENAME_SIZE;

		//Create file allocation header:
		EEFS_FileAllocationTableHeader_t table_header;
		create_file_allocation_table_header(table_header, size, 64);

		//Burn table header to storage:
		EEFS_LIB_EEPROM_WRITE((void*)offset, (void*)&table_header, sizeof(EEFS_FileAllocationTableHeader_t));

	}

	uint8_t volume_exist(const char* volume, uint32_t offset){
		char buffer[EEFS_MAX_DEVICENAME_SIZE];
		EEFS_LIB_EEPROM_READ((void*)buffer, (void*)offset, EEFS_MAX_DEVICENAME_SIZE);
		if(strcmp(buffer, volume) != 0) return 0;
		else return 1;
	}

	char load(char* filename, uint8_t* buffer, uint8_t size){
		char full_filename[40];

		strcpy(full_filename, volume_name);
		strcat(full_filename,"/");
		strcat(full_filename, filename);

		int32_t descriptor =  EEFS_Open(full_filename, O_RDWR);

		//Check if file even exists:
		if(descriptor < 0){
			return descriptor;
		}

		EEFS_Read(descriptor, buffer, size);
		EEFS_Close(descriptor);

		return 0;
	}

	int8_t save(char* filename, uint8_t* buffer, uint8_t size){
		char full_filename[40];

		strcpy(full_filename, volume_name);
		strcat(full_filename,"/");
		strcat(full_filename, filename);

		uint32_t descriptor =  EEFS_Open(full_filename, O_RDWR);

		//Check if file even exists:
		if(descriptor < 0){
			return descriptor;
		}

		EEFS_Write(descriptor, buffer, size);
		EEFS_Close(descriptor);
	}

	int8_t create_file(char* filename, uint8_t* data, uint8_t size){
		char full_filename[40];

		strcpy(full_filename, volume_name);
		strcat(full_filename,"/");
		strcat(full_filename, filename);

		int32_t descriptor = EEFS_Creat(full_filename, EEFS_ATTRIBUTE_NONE);

		EEFS_Write(descriptor, data, size);
		return EEFS_Close(descriptor);
	}

	void create_file_allocation_table_header(EEFS_FileAllocationTableHeader_t& header,
											 uint32_t data_size, uint8_t max_no_files){

		header.Crc 				= 0;
		header.Magic 			= EEFS_FILESYS_MAGIC;
		header.Version 			= 1;
		header.FreeMemoryOffset = sizeof(EEFS_FileAllocationTableHeader_t)+
								  max_no_files*sizeof(EEFS_FileAllocationTableEntry_t);
		header.FreeMemorySize   = data_size - header.FreeMemoryOffset;
		header.NumberOfFiles	= 0;
	}

	void filesystem_not_found_state(){
		while(1){
			uint8_t i;
			i++;
		}
	}

private:
	const char* volume_name;
	uint32_t name_offset;
	uint32_t header_offset;
	uint32_t size;
	EEFS_Device_t* eefs_device;
};



