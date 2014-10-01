#include "stm32f4xx.h"
#include "Communication.hpp"
#include "ControlInput.hpp"
#include "FlightControl.hpp"
#include "FlightDynamics.hpp"
#include "FlightNavigation.hpp"
#include "SystemStatus.hpp"

#include "Framework.hpp"
#include "Global.hpp"

#include "MPU9150.hpp"
#include "BMP085.hpp"
#include "HeliosLED.hpp"
#include "24LC64.hpp"
#include "HC-SR04.hpp"

//TODO-JWA: These definitely shouln't be defined here!
template<>
uint16_t Global<float>::no_globals = 0;
template<>
Global<float>* 	Global<float>::global_table[FRAMEWORK_MAX_GLOBALS];

uint32_t 	EEPROM::counter = 1000;
EEPROM* 	EEPROM::EEPROM_table[10];
uint8_t 	EEPROM::table_counter = 0;


int main(void){

	MPU9150 mpu9150;
	HeliosLED leds;
	EEPROM_24LC64 eeprom;
	HC_SR40 sonar;

	float x = 0, y = 0, z = 0;
	float gx = 0, gy = 0, gz = 0, gy_last = 0;
	float altitude = 0;

	uint8_t in_buffer[10] = {1,2,3,4,5,6,7,8,9,10};
	uint8_t out_buffer[10] = {0};

	//eeprom.write(0, 10, in_buffer);
	eeprom.read(0, 10, out_buffer);

	while(1){
		mpu9150.get_acc_data(x,y,z);
		mpu9150.get_gyro_data(gx,gy,gz);

		if(gy_last == gy)
			leds.set_LED(DEBUG_BLUE1, LED_ON);
		else
			leds.set_LED(DEBUG_BLUE1, LED_OFF);

		altitude = sonar.get_altitude();

		gy_last = gy;
		Time.delay_ms(20);
	}

	BMP085 bmp085;
	float temperature;
	while(1){
		if(bmp085.dataAvailable()){
			temperature = bmp085.getAltitude();
		}
		Time.delay_ms(150);
	}



	//Create Application modules ------------------------------------------:
	Communication 		communication("Communication", 			configMINIMAL_STACK_SIZE * 10, 	1, 1000);
	ControlInput 		control_input("ControlInput", 			configMINIMAL_STACK_SIZE, 		1, 1000);
	FlightControl 		flight_control("FlightControl", 		configMINIMAL_STACK_SIZE, 		1, 1000);
	FlightDynamics 		flight_dynamics("FlightDynamics", 		configMINIMAL_STACK_SIZE, 		1, 1000);
	FlightNavigation 	flight_navigation("FlightNavigations", 	configMINIMAL_STACK_SIZE, 		1, 1000);

	SystemStatus 		system_status("SystemStatus", 			configMINIMAL_STACK_SIZE, 		1, 1000,
									  &communication, &control_input, &flight_control, &flight_dynamics, &flight_navigation);


	//Create Drivers:
	//MPU9150 mpu9150;

	//Bind Application Interfaces ------------------------------------------:
	APP_Attitude_I attitude({ &control_input.attitude,	  //PROVIDER
							  &flight_control.attitude,   //CONSUMER
							  &flight_navigation.attitude,//CONSUMER
							  &communication.attitude});  //CONSUMER

	APP_Attitude_I attitude1({&communication.attitude});


	//Bind HAL Interfaces ------------------------------------------::
	flight_dynamics.set_accelerometer(&mpu9150);


	//Check that EEPROM table is not changed:
	//EEPROM::Synchronize()

	//Start Scheduler:
	schedulerStart();

	while(1);
}
