#pragma once

#include "Framework.hpp"

struct AccelerometerCalibration{
	float OffsetX;
	float OffsetY;
	float OffsetZ;
};

class HAL_Accelerometer_I : public HAL_Interface {
	public:
		virtual bool is_alive() = 0;
		virtual bool is_error() = 0;
		virtual int16_t get_data(float&, float&, float&) = 0;
		virtual void    restart() = 0;
		virtual void    set_calibration(AccelerometerCalibration&) = 0;
		virtual void    get_calibration(AccelerometerCalibration&) = 0;
};
