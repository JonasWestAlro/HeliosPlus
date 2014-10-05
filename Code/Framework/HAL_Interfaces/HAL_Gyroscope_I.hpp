
#pragma once

#include "Framework.hpp"

struct GyroscopeCalibration{
	float offset_x;
	float offset_y;
	float offset_z;
};

class HAL_Gyroscope_I : public HAL_Interface {
	public:
		virtual bool 	is_alive() = 0;
		virtual bool 	is_error() = 0;
		virtual void    restart() = 0;
		virtual int16_t get_gyro_data(float&, float&, float&) = 0;
		virtual void    set_gyro_calibration(GyroscopeCalibration&) = 0;
		virtual void    get_gyro_calibration(GyroscopeCalibration&) = 0;
		virtual void 	start_gyro_calibration(void) = 0;
		virtual void 	stop_gyro_calibration(void) = 0;
};

