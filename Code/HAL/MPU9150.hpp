#pragma once

#include "Framework.hpp"

class MPU9150 : public HAL_Accelerometer_I{

	public:
		MPU9150(){

		}

	    virtual bool is_alive(){
	    	return true;
	    };
	    virtual bool is_error(){
	    	return false;
	    };
	    virtual int16_t get_data(float& x, float& y, float& z){};
	    virtual void    restart(){};
	    virtual void    set_calibration(AccelerometerCalibration&){}
	    virtual void    get_calibration(AccelerometerCalibration&){}

};
