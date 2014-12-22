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

#include "eefs_filesys.h"


void test_sensors();
void test_eeprom(uint8_t numbers_at_the_time, uint8_t number_of_times);

uint8_t eeprom_simulation[10000] = {0};
int32_t filedescriptor = 0;
int32_t filedescriptor1 = 0;

EEFS_FileAllocationTableHeader_t header = {
		0,
		EEFS_FILESYS_MAGIC,
		1,
		100,
		9000,
		0
};

char buffer_in[18] = "THIS IS MY FILE!!";
char buffer_out[18] = {0};
int8_t result = 0;

void test_filesystem(){
	memcpy(eeprom_simulation, &header, sizeof(EEFS_FileAllocationTableHeader_t));

	result = EEFS_InitFS("/EEDEV0", (uint32_t)&eeprom_simulation);
	result = EEFS_Mount("/EEDEV0", "/EEFS0");

	filedescriptor = EEFS_Creat("/EEFS0/My_file", EEFS_ATTRIBUTE_NONE);
	EEFS_Write(filedescriptor, buffer_in, 18);
	EEFS_Close(filedescriptor);

	Time.delay_ms(1000);

	filedescriptor1 =  EEFS_Open("/EEFS0/My_file", O_RDWR);
	EEFS_Read(filedescriptor1, buffer_out, 18);

	while(1){
		int i = 0;
		i++;
	}
}

int main(void){

	//test_filesystem();
	//test_sensors();

	WirelessUART wireless_uart;
	Debug.set_driver(&wireless_uart);

	//test_eeprom(100, 100);

	//Create drivers ----------------------------------------------------:
	MPU9150 	  mpu9150;
	HeliosLED 	  leds;
	PPMReceiver   control_receiver;
	Motor 		  motors;
	GPS_UART 	  gps_uart;
	HC_SR40 	  sonar;
	BMP085 		  barometer;
	EEPROM_24LC64 eeprom;

	//Initialize storage and load global variables:
	GlobalFileSystemHandler::get_instance().set_driver(&eeprom);
	Globals::init();

	//Create Application modules ------------------------------------------:
	Communication 		communication("/Communication", 		configMINIMAL_STACK_SIZE*10, 	1, 1000);
	ControlInput 		control_input("/ControlInput", 			configMINIMAL_STACK_SIZE*5, 	1, 1000);
	FlightControl 		flight_control("/FlightControl", 		configMINIMAL_STACK_SIZE*6, 	1, 1000);
	FlightDynamics 		flight_dynamics("/FlightDynamics", 		configMINIMAL_STACK_SIZE*20, 	1, 1000);
	FlightNavigation 	flight_navigation("/FlightNavigations", configMINIMAL_STACK_SIZE*5, 	1, 1000);
	SystemStatus 		system_status("/SystemStatus", 			configMINIMAL_STACK_SIZE*10, 	1, 1000,
									  	  	  	  	  	  	  	&communication, &control_input, &flight_control, &flight_dynamics, &flight_navigation);
	CLI 				cli("/CLI", 							configMINIMAL_STACK_SIZE*10, 	1, 1000,
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

	APP_Control_I control2({&cli.control_socket});

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

	//Start Scheduler:
	schedulerStart();

	while(1);
}


void test_eeprom(uint8_t numbers_at_the_time, uint8_t number_of_times){

	MPU9150 mpu9150;
	HeliosLED leds;
	HC_SR40 sonar;
	EEPROM_24LC64 eeprom;

	uint8_t success = 1;
	uint8_t read_failed = 0;
	uint8_t write_failed = 0;
	uint8_t in_buffer[100];
	uint8_t out_buffer[100] = {0};

	uint8_t c;
	uint8_t i;

	for(i=0; i<numbers_at_the_time; i++){
		in_buffer[i] = i;
	}

	while(1){
		for(i=0; i<number_of_times; i++){
			for(uint8_t t = 0; t<numbers_at_the_time; t++){
				out_buffer[t] = 0;
			}
			read_failed  = 0;
			write_failed = 0;

			if(!eeprom.write(numbers_at_the_time*i, numbers_at_the_time, in_buffer)){
				write_failed = 1;
			}

			if(!eeprom.read(numbers_at_the_time*i, numbers_at_the_time, out_buffer)){
				read_failed = 1;
			}

			success = 1;
			for(uint8_t t = 0; t<numbers_at_the_time; t++){
				if(in_buffer[t] == out_buffer[t]){
					//OK!
				}else{
					success = 0;
					break;
				}
			}


			Debug.send_number((uint32_t)i);
			Debug.send('.');
			Debug.put(" checking ");
			Debug.send_number((uint32_t)numbers_at_the_time*i);
			Debug.put(" -> ");
			Debug.send_number((uint32_t)numbers_at_the_time*i+numbers_at_the_time);
			if(success && !write_failed && !read_failed){
				Debug.put_and_transmit(" --- OK\n\r");
			}else{
				Debug.put(" is NOT OK .... ");
				if(write_failed) Debug.put(" Write failed.. ");
				if(read_failed) Debug.put(" Read failed... ");
				Debug.put_and_transmit("\n\r");
			}
		}

		while(!Debug.data_available());
		Debug.receive(&c);
		Debug.put_and_transmit("\n\n\r");
		Debug.put_and_transmit("RUNNING NEW TEST...\n\r");
	}
}


void test_sensors(){

	MPU9150 mpu9150;
	HeliosLED leds;
	HC_SR40 sonar;

	float x = 0, y = 0, z = 0;
	float gx = 0, gy = 0, gz = 0, gy_last = 0;
	float altitude = 0;

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
