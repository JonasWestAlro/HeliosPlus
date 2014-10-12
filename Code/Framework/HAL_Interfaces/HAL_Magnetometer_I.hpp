#pragma once

#include "HAL_Interface.hpp"

typedef struct{
	float offset_X;
	float offset_Y;
	float offset_Z;
	float range_X;
	float range_Y;
	float range_Z;
}MagnetometerCalibration;

class HAL_Magnetometer_I : public HAL_Interface {
	public:
	virtual bool is_alive(void) = 0;
	virtual bool is_error(void) = 0;
	virtual void 	restart(void) = 0;

	virtual int16_t get_mag_data(float&, float&, float&) = 0;
	virtual int16_t	get_raw_mag_data(int16_t*) = 0;
	virtual void  	get_mag_calibration(MagnetometerCalibration*) = 0;
	virtual void	set_mag_calibration(MagnetometerCalibration*) = 0;

	virtual void 	start_mag_calibration(void) = 0;
	virtual void 	stop_mag_calibration(void) = 0;
};
