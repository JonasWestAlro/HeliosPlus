#pragma once

#include "Framework.hpp"
#include "Vector.hpp"
#include "nmea.h"

#define GRAVITY_MSS -1.05

enum ALTITUDE_SENSOR{
	SENSOR_SONAR,
	SENSOR_BAROMETER
};

class FlightNavigation : public ApplicationModule {
	public:
		FlightNavigation(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);

		APP_Attitude_I 	 attitude_socket;
		APP_Navigation_I navigation_socket;

		void set_accelerometer(HAL_Accelerometer_I* acc){accelerometer = acc;}
		void set_GPS(HAL_Communication_I* GPS_){GPS = GPS_;}
		void set_barometer(HAL_Altitude_I* baro){barometer = baro;}
		void set_sonar(HAL_Altitude_I* sonar_){sonar = sonar_;}
	protected:
		void task(void);
		void handle_message(Message& msg);

	private:
		HAL_Accelerometer_I* 	accelerometer;
		HAL_Communication_I*	GPS;
		HAL_Altitude_I*  	 	barometer;
		HAL_Altitude_I*  	 	sonar;

		//Sensor Readings
		float sonar_altitude = 0;
		float baro_altitude  = 0;
		float baro_filtered_altitude  = 0;
		float baro_offset = 0;
		float baro_offset_filtered = 0;
		Coordinate accel_earth_frame;
		float dt = 0;

		//Estimates:
		float altitude 			= 0;
		float altitude_filtered = 0;
		float altitude_velocity = 0;
		float sonar_offset 	    = 4.5;

		//Sensor states:
		STATUS gps_status   = STATUS_NOTOK;
		STATUS baro_status  = STATUS_NOTOK;
		STATUS acc_status   = STATUS_NOTOK;
		STATUS sonar_status = STATUS_NOTOK;

		//The active sensor:
		ALTITUDE_SENSOR active_sensor = SENSOR_SONAR;

		void update_status(void);
		void report_status(void);

		void estimate_altitude(void);
		void estimate_altitude_with_sonar(void);
		void estimate_altitude_with_baro(void);
		void try_sensor_transition(void);

		void update_baro(void);
		void update_acc(void);
		void update_gps(void);
		void update_sonar(void);
};

