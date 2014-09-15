#pragma once

#include "Framework.hpp"

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
	virtual uint8_t is_alive(void) = 0;
	virtual uint8_t is_error(void) = 0;
	virtual int16_t get_data(float*, float*, float*) = 0;
	virtual int16_t	get_raw_data(int16_t*) = 0;
	virtual void 	restart(void) = 0;
	virtual void  	get_calibration(MagnetometerCalibration*) = 0;
	virtual void	set_calibration(MagnetometerCalibration*) = 0;
};
