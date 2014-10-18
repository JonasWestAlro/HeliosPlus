#pragma once

#include "Filesystem.hpp";

enum{
	FILE_SAVED = 0,
	FILE_CREATED,
	FILE_LOADED
};

class Storable{
public:
	Storable(FileSystem* filesystem_, char* filename_){
		filename = filename_;
		filesystem = filesystem_;
	}

	void make_storable(FileSystem* filesystem_, char* filename_){
		filename = filename_;
		filesystem = filesystem_;
	}

	char* get_filename(){return filename;}

	virtual uint8_t save(uint8_t* buffer){
		if(filesystem == 0) return false;

		serialize(buffer);

		int8_t result = filesystem->save(filename, buffer, get_size());

		if(result == EEFS_FILE_NOT_FOUND){
			result = filesystem->create_file(filename, buffer, get_size());

			//Try again:
			int8_t result = filesystem->save(filename, buffer, get_size());
			return FILE_CREATED;
		}

		return FILE_SAVED;
	};

	virtual uint8_t load(uint8_t* buffer){
		if(filesystem == 0) return false;

		int8_t result = filesystem->load(filename, buffer, get_size());

		if(result == EEFS_FILE_NOT_FOUND){
			serialize(buffer);
			filesystem->create_file(filename, buffer, get_size());
			return FILE_CREATED;
		}else{
			deserialize(buffer);
			return FILE_LOADED;
		}
	};

	virtual uint32_t serialize(uint8_t*) = 0;
	virtual void 	 deserialize(uint8_t*) = 0;
	virtual uint32_t get_size() = 0;

	char* 		filename;
	FileSystem* filesystem;
};
