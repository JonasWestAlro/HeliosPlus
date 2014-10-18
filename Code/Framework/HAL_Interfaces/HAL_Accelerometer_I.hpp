#pragma once

#include "HAL_Interface.hpp"
#include "Storable.hpp"
#include "SerializeHelper.hpp"

struct AccelerometerCalibration : public Storable{
	float offset_x;
	float offset_y;
	float offset_z;

	AccelerometerCalibration(): Storable(0, "0"){reset();}

	void reset(){
		offset_x = 0; offset_y = 0; offset_z = 0;
	}

	virtual uint32_t serialize(uint8_t* buffer){
		uint8_t* start_ptr = buffer;

		buffer += SerializeHelper::serialize_float(buffer, offset_x);
		buffer += SerializeHelper::serialize_float(buffer, offset_y);
		buffer += SerializeHelper::serialize_float(buffer, offset_z);

		//return size of object:
		return buffer-start_ptr;
	};

	virtual void deserialize(uint8_t* buffer){
		buffer += SerializeHelper::deserialize_float(buffer, offset_x);
		buffer += SerializeHelper::deserialize_float(buffer, offset_y);
		buffer += SerializeHelper::deserialize_float(buffer, offset_z);
	};

	virtual uint32_t get_size(){return 3*sizeof(float);};
};

class HAL_Accelerometer_I : public HAL_Interface {
	public:
		virtual bool 	is_alive() = 0;
		virtual bool 	is_error() = 0;
		virtual void    restart() = 0;
		virtual int16_t get_acc_data(float&, float&, float&) = 0;
		virtual void    set_acc_calibration(AccelerometerCalibration&) = 0;
		virtual void    get_acc_calibration(AccelerometerCalibration&) = 0;
		virtual void 	start_acc_calibration(void) = 0;
		virtual void 	stop_acc_calibration(void) = 0;
};
