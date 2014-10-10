#include "FlightNavigation.hpp"

FlightNavigation::FlightNavigation(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size)
: ApplicationModule(name, stackSize, priority, eeprom_size)
{
	// Initializer code goes here
}

void FlightNavigation::task(void){
	/*static uint32_t timestamp = 0;

	//Get the current attitude of the system:
	attitude_socket.receive();

	//Calculate _dt
	dt = Time.get_time_since_sec(timestamp);
	timestamp = Time.get_timestamp();

	//Get new sensor data:
	update_acc();
	update_baro();
	update_gps();
	update_sonar();

	//estimate altitude:
	estimate_altitude();

	//---------   PUBLISH  -------------
	navigation_socket.acceleration.z   = accel_earth_frame.z;
	navigation_socket.velocity.z 	   = altitude_velocity;
	navigation_socket.position.z 	   = altitude;
	navigation_socket.publish();*/
}

void FlightNavigation::handle_message(Message& msg){}
void FlightNavigation::update_status(void){}
void FlightNavigation::report_status(void){}

void FlightNavigation::estimate_altitude(void){
	if(sonar_status == STATUS_OK){
		if(active_sensor == SENSOR_SONAR)
			estimate_altitude_with_sonar();
		else
			try_sensor_transition();
	}else{
		estimate_altitude_with_baro();
	}
}

void FlightNavigation::estimate_altitude_with_sonar(void){
	static uint32_t timestamp = 0;
	static float last_sonar_altitude = 0;
	float alt_dif;
	float vel_est;

	//Compute time since last estimate with sonar
	float dt = Time.get_time_since_sec(timestamp);

	//compute the estimated velocity:
	//if it hasn't been too long since last sample:
	if(dt < 0.1){
		alt_dif = sonar_altitude - last_sonar_altitude;
		vel_est = alt_dif/dt;
	}else{
		vel_est = 0;
	}

	//filter velocity estimate:
	altitude_velocity = 0.95*altitude_velocity + 0.05*vel_est;

	//update barometer offset:
	baro_offset += sonar_altitude - baro_filtered_altitude;
	baro_offset_filtered = baro_offset_filtered*0.999 + baro_offset*0.001;

	//assign the altitude.. No filtering needed.
	altitude = sonar_altitude;

	//TODO-JWA: Do edge-detection to prevent jumps..

	last_sonar_altitude = sonar_altitude;

	active_sensor = SENSOR_SONAR;
	timestamp = Time.get_timestamp();
}

void FlightNavigation::estimate_altitude_with_baro(void){
	static float acc_filtered = 0;

	//Filter accelerometer
	acc_filtered = 0.95*acc_filtered + 0.05*accel_earth_frame.z;

	//Integrate to find velocity:
	//(Note: the "2" is a magic number making it more like the sonar estimator).
	altitude_velocity += acc_filtered*dt*2;

	//Highpass velocity:
	altitude_velocity *= 0.95;

	//Complementary altitude:
	altitude = 0.99*altitude + 0.01*baro_filtered_altitude;// + (_altitude_velocity*3.0);

	active_sensor = SENSOR_BAROMETER;
}

void FlightNavigation::try_sensor_transition(void){
	//The sonar is OK, but we've been using the barometer lately. So we'll
	//make sure that there isn't a jump in altitude when the transistion happens.

	//We'll make sure that the current altitude and the measured one by the
	//sonar is similar. This is to avoid sudden changes in the altitude
	//estimation.
	float dif = sonar_altitude - altitude;
	if(dif > 20 || dif < -20){
		//The altitude is not close enough to make the sensor switch!
		//Therefore we'll try and calibrate the barometer slowly to a new
		//offset that matches what the sonar is measuring.
		if(dif < 0)
			baro_offset -= 0.08;
		else
			baro_offset += 0.08;

		estimate_altitude_with_baro();
	}else{
		//the sensor values are close enough to make the transistion:
		estimate_altitude_with_sonar();
	}
}

void FlightNavigation::update_baro(void){
    //Download new sample, and convert to cm:
	portENTER_CRITICAL();
		if(barometer->data_available()){
			baro_altitude = barometer->get_altitude()*100;
			baro_filtered_altitude = 0.95*baro_filtered_altitude + 0.05*(baro_altitude + baro_offset);
		}
	portEXIT_CRITICAL();
}

void FlightNavigation::update_acc(void){
	Vector raw_acc;
	Vector acc_earth_frame_temp;

	/* Download Accelerometer data if available:  */
	if(!accelerometer->get_acc_data(raw_acc.x,  raw_acc.y, raw_acc.z)){
		acc_status = STATUS_NOTOK;
	}else acc_status = STATUS_OK;

	//Convert accelerometer to earth frame
	Vector angles(attitude_socket.pitch, attitude_socket.roll, 0);
	raw_acc.convert_to_frame(angles);

	accel_earth_frame.z = raw_acc.z;

	//remove influence of gravity and convert to cm/s^2
	accel_earth_frame.z += GRAVITY_MSS;
	accel_earth_frame.z *= 9.82*100;
}

void FlightNavigation::update_gps(void){
	uint8_t c;
	static uint32_t timestamp;

	if(Time.get_time_since_sec(timestamp) > 5){
		gps_status = STATUS_NOTOK;
	}

	while(GPS->data_available()){
		GPS->receive(&c);
		// parse to nmea:
		/*if(nmea_parse(c)) {
			if (nmea_gprmc_status() == 'A') {
				navigation_socket.longitude = nmea_gprmc_longitude();
				navigation_socket.latitude  = nmea_gprmc_latitude();
				navigation_socket.speed 	= nmea_gprmc_speed(KMPH);
				navigation_socket.course 	= nmea_gprmc_course();
			}
		}

		gps_status = STATUS_OK;
		timestamp = Time.get_timestamp();*/
	}
}

void FlightNavigation::update_sonar(void){
	//Download Sonar data if available:
	if(sonar->get_status() == STATUS_OK){
		//Get altitude:
		sonar_altitude 	= sonar->get_altitude();
		sonar_status = STATUS_OK;
	}else{
		sonar_status = STATUS_NOTOK;
	}
}
