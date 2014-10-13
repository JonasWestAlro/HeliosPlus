#pragma once

#include "Framework.hpp"
#include "EEPROM_I2C.hpp"

#define EEPROM_ADDRESS 			0b1010101

class EEPROM_24LC64 : public HAL_Eeprom_I{

	public:
	EEPROM_24LC64(): i2c(EepromI2C::get_instance()){

		/*
		 * setup WP pin
		 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 				// we are going to use PD0
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		// set GPIO speed
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// enable pull up resistors
		GPIO_Init(GPIOD, &GPIO_InitStruct);					// init GPIOD

	}

	virtual uint8_t write(uint16_t registeradress, uint8_t n, uint8_t* buffer){
		uint16_t numOfpages;
		uint8_t firstSize, lastSize;
		uint8_t i;

		firstSize = 32 - (registeradress % 32);

		if(firstSize < n){
			lastSize = (n - firstSize) % 32;
			numOfpages = (n - firstSize) / 32;
		}
		else {
			numOfpages = 0;
			firstSize = n;
			lastSize = 0;
		}

		// Write first page
		if(firstSize != 0) {
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, firstSize, buffer)) return 0;
			registeradress += firstSize;
			buffer += firstSize;
			Time.delay_ms(5);
		}

		// Write pages
		for(i = 0; i<numOfpages; i++){
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, 32, buffer)) return 0;
			registeradress += 32;
			buffer += 32;

			//TODO-JWA: Is this really necessary?
			Time.delay_ms(5);
		}

		// Write last page
		if(lastSize != 0) {
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, lastSize, buffer)) return 0;
		}

		return 1;
	}

	virtual uint8_t read(uint16_t register_adress, uint8_t n, uint8_t* result){
		return i2c.read_register16(EEPROM_ADDRESS, register_adress, n, result);
	}


	//TODO-JWA: This can be optimized, it's a very fast "copy-past" implementation!
	virtual uint8_t erase_all(){
		uint16_t registeradress = 0;
		uint16_t n = 60000;
		uint8_t buffer[4] = {0};

		uint16_t numOfpages;
		uint8_t firstSize, lastSize;
		uint8_t i;

		firstSize = 32 - (registeradress % 32);

		if(firstSize < n){
			lastSize = (n - firstSize) % 32;
			numOfpages = (n - firstSize) / 32;
		}
		else {
			numOfpages = 0;
			firstSize = n;
			lastSize = 0;
		}

		// Write first page
		if(firstSize != 0) {
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, firstSize, buffer)) return 0;
			registeradress += firstSize;
			Time.delay_ms(5);
		}

		// Write pages
		for(i = 0; i<numOfpages; i++){
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, 32, buffer)) return 0;
			registeradress += 32;

			//TODO-JWA: Is this really necessary?
			Time.delay_ms(5);
		}

		// Write last page
		if(lastSize != 0) {
			if(!i2c.write_register16(EEPROM_ADDRESS, registeradress, lastSize, buffer)) return 0;
		}

		return 1;
	}


	private:
		GenericI2C& i2c;

};
