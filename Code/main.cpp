#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "CLI.hpp"
#include "Communication.hpp"
#include "ControlInput.hpp"
#include "FlightControl.hpp"
#include "FlightDynamics.hpp"
#include "FlightNavigation.hpp"
#include "SystemStatus.hpp"

#include "Framework.hpp"
#include "Global.hpp"


//HAL Drivers includes:
#include "Motor.hpp"
#include "MPU9150.hpp"
#include "BMP085.hpp"
#include "HeliosLED.hpp"
#include "24LC64.hpp"
#include "HC-SR04.hpp"
#include "UART.hpp"
#include "WirelessUART.hpp"
#include "PPMReceiver.hpp"
#include "GPS_UART.hpp"

#include "Debug.hpp"

EepromHandler* ApplicationModule::default_eeprom_handler;

void test_sensors();

int main(void){
	//test_sensors();

	WirelessUART wireless_uart;
	Debug.set_driver(&wireless_uart);

	//Create drivers ----------------------------------------------------:
	MPU9150 	  mpu9150;
	HeliosLED 	  leds;
	PPMReceiver   control_receiver;
	Motor 		  motors;
	GPS_UART 	  gps_uart;
	HC_SR40 	  sonar;
	BMP085 		  barometer;
	EEPROM_24LC64 eeprom;


	//Create EEPROM Table:
	EepromHandler eeprom_handler(&eeprom);
	ApplicationModule::set_defualt_eeprom_handler(&eeprom_handler);


	//Create Application modules ------------------------------------------:
	Communication 		communication("Communication", 			configMINIMAL_STACK_SIZE*10, 	1, 1000);
	ControlInput 		control_input("ControlInput", 			configMINIMAL_STACK_SIZE*5, 	1, 1000);
	FlightControl 		flight_control("FlightControl", 		configMINIMAL_STACK_SIZE*6, 	1, 1000);
	FlightDynamics 		flight_dynamics("FlightDynamics", 		configMINIMAL_STACK_SIZE*20, 	1, 1000);
	FlightNavigation 	flight_navigation("FlightNavigations", 	configMINIMAL_STACK_SIZE*5, 	1, 1000);

	SystemStatus 		system_status("SystemStatus", 			configMINIMAL_STACK_SIZE*10, 		1, 1000,
									  &communication, &control_input, &flight_control, &flight_dynamics, &flight_navigation);

	CLI 				cli("CLI", configMINIMAL_STACK_SIZE*10, 	1, 1000,
							&communication, &control_input, &flight_control, &flight_dynamics, &flight_navigation, &system_status);




	//Bind Application Interfaces ------------------------------------------:
	APP_SystemStatus_I systemstatus({&system_status.system_status_socket,
									 &communication.system_status,
									 &control_input.system_status_socket,
									 &flight_control.system_status_socket,
									 &cli.system_status_socket});

	APP_Attitude_I attitude({ &flight_dynamics.attitude_socket,   //PROVIDER
							  &flight_navigation.attitude_socket, //CONSUMER
							  &flight_control.attitude_socket});  //CONSUMER

	APP_Control_I control({&control_input.control_socket,
						   &flight_control.control_socket});

	APP_Control_I control1({&communication.control_socket});



	//Bind HAL Interfaces ------------------------------------------::
	flight_dynamics.set_accelerometer(&mpu9150);
	flight_dynamics.set_gyroscope(&mpu9150);
	flight_dynamics.set_magnetometer(&mpu9150);
	flight_dynamics.set_leds(&leds);
	flight_dynamics.set_debug(&gps_uart);

	control_input.set_control_receiver(&control_receiver);

	flight_control.set_motors(&motors);

	flight_navigation.set_accelerometer(&mpu9150);
	flight_navigation.set_GPS(&gps_uart);
	flight_navigation.set_barometer(&barometer);
	flight_navigation.set_sonar(&sonar);

	system_status.set_leds(&leds);

	communication.set_driver(&gps_uart);

	//Check that EEPROM table is not changed:
	eeprom_handler.synchronize();

	//Start Scheduler:
	schedulerStart();

	while(1);
}

void test_sensors(){

	MPU9150 mpu9150;
	HeliosLED leds;
	EEPROM_24LC64 eeprom;
	HC_SR40 sonar;

	float x = 0, y = 0, z = 0;
	float gx = 0, gy = 0, gz = 0, gy_last = 0;
	float altitude = 0;

	uint8_t in_buffer[10] = {1,2,3,4,5,6,7,8,9,10};
	uint8_t out_buffer[10] = {0};

	eeprom.write(0, 10, in_buffer);
	eeprom.read(0, 10, out_buffer);

	leds.set_LED(DEBUG_BLUE1, LED_OFF);
	if(out_buffer[0] == 1){
		leds.set_LED(DEBUG_BLUE1, LED_ON);
	}
	while(1);

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
		if(bmp085.data_available()){
			temperature = bmp085.get_altitude();
		}
		Time.delay_ms(150);
	}

}
