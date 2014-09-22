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

//TODO-JWA: These definitely shouln't be defined here!
template<>
uint16_t Global<float>::no_globals = 0;
template<>
Global<float>* 	Global<float>::global_table[FRAMEWORK_MAX_GLOBALS];

uint32_t 	EEPROM::counter = 1000;
EEPROM* 	EEPROM::EEPROM_table[10];
uint8_t 	EEPROM::table_counter = 0;


int main(void){

	BMP085 bmp085;
	float temperature;
	while(1){
		if(bmp085.dataAvailable())
			temperature = bmp085.getAltitude();
		Time.delay_ms(150);
	}

	MPU9150 mpu9150;

	float x = 0, y = 0, z = 0;

	while(1){
		mpu9150.get_data(x,y,z);
		Time.delay_ms(20);
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
