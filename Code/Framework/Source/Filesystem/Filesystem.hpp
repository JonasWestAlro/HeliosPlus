#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "eefs_filesys.h"
#include "HAL_Eeprom_I.hpp"

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

private:
	GlobalFileSystemHandler(){}

	HAL_Eeprom_I* 	storage_device;

	FileSystem*  	filesystem_table[10];
	uint32_t 		offset_counter = 0;
	uint8_t  		table_counter = 0;
};

extern uint8_t eeprom_buffer[10000];

class FileSystem{
public:
	FileSystem(const char* volume, uint32_t size_):
		volume_name(volume){

		int32 result = 0;

		size = size_;

		//Find offset from GlobalFilesystemHandler:
		offset =  (uint32_t)&eeprom_buffer +
							GlobalFileSystemHandler::get_instance().add_new_filesystem(this, size);

		//Initialize filesystem:
		result = EEFS_InitFS(const_cast<char*>(volume_name), offset);

		//If the filesystem doesn't exist:
		if(result == EEFS_NO_SUCH_DEVICE){
			//We have to create the device..
			EEFS_FileAllocationTableHeader_t table_header;
			create_file_allocation_table_header(table_header, size, 64);

			//Burn table header to storage:
			EEFS_LIB_EEPROM_WRITE((void*)offset, (void*)&table_header, sizeof(EEFS_FileAllocationTableHeader_t));

			//Now try and init again..
			result = EEFS_InitFS(const_cast<char*>(volume_name), offset);

			if(result == EEFS_NO_SUCH_DEVICE){
				//Something still went wrong..
				while(1);
			}
		}

		eefs_device = EEFS_GetDevicePtr(result);

		result = EEFS_Mount(const_cast<char*>(volume_name), const_cast<char*>(volume_name));
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

private:
	const char* volume_name;
	uint32_t offset;
	uint32_t size;
	EEFS_Device_t* eefs_device;
};



