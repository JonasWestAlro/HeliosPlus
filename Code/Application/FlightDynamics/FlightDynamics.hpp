#pragma once

#include "Framework.hpp"
#include "EKF_Attitude.hpp"
#include "Quaternion.hpp"
#include "EulerAngle.hpp"
#include "IIR_Filter.hpp"
#include "mavlink.h"

extern "C" {
    #include "Attitude_ExtendedKalman.h"
}


class FlightDynamics : public ApplicationModule{
	public:
		FlightDynamics(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);

		void set_accelerometer(HAL_Accelerometer_I* acc){accelerometer = acc;}
		void set_gyroscope(HAL_Gyroscope_I* gyro){gyroscope = gyro;}
		void set_magnetometer(HAL_Magnetometer_I* mag){magnetometer = mag;}
		void set_leds(HAL_HeliosLED_I* leds_){leds = leds_;}
		void set_debug(HAL_Communication_I* debug_){debug = debug_;}

		APP_Attitude_I   attitude_socket;
		APP_Quaternion_I attitude_quarternion_socket;
	protected:
		void run(void);

	private:
		portTickType xLastWakeTime;

		//HAL Interfaces:
		HAL_Accelerometer_I* accelerometer;
		HAL_Gyroscope_I* 	 gyroscope;
		HAL_Magnetometer_I*  magnetometer;
		HAL_HeliosLED_I* 	 leds;
		HAL_Communication_I* debug;

		uint32_t 			 no_sensor_errors;

		//Sensor status:
		STATUS sensor_status = STATUS_NOTOK;

		//Sensor fusion variables:
		EKF_Attitude EKF;
		Quaternion attitude_quaternion;
		EulerAngle attitude_euler;

		float raw_gyro[3] = {0};
		float raw_acc[3] = {0};
		float raw_mag[3] = {0};

		IIR_Filter<4> pitch_filter;
		IIR_Filter<4> roll_filter;
		IIR_Filter<4> yaw_filter;

		struct EEPROM_Structure{
			AccelerometerCalibration accelerometer_calibration;
		};

		EEPROM_Structure eeprom_structure;

		void handle_message(Message& msg);
		void update_sensor_data();
		void report_status(){}
		void update_status(){}
		void debug_led();
};

