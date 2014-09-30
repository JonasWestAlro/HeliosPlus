#pragma once

#include "Framework.hpp"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

typedef struct {
	uint8_t led_ID;
	GPIO_TypeDef* port;
	uint16_t pin;
}LED_LUTentry;

class HeliosLED : public HAL_HeliosLED_I{

	public:
		HeliosLED(){
			/* STM32F4 discovery LEDs */
			GPIO_InitTypeDef LED_Config;

			/* Always remember to turn on the peripheral clock...  If not, you may be up till 3am debugging... */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

			LED_Config.GPIO_Pin = GPIO_Pin_5;
			LED_Config.GPIO_Mode = GPIO_Mode_OUT;
			LED_Config.GPIO_OType = GPIO_OType_PP;
			LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
			LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOC, &LED_Config);

			LED_Config.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_11;
			LED_Config.GPIO_Mode = GPIO_Mode_OUT;
			LED_Config.GPIO_OType = GPIO_OType_PP;
			LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
			LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOE, &LED_Config);

			LED_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
			LED_Config.GPIO_Mode = GPIO_Mode_OUT;
			LED_Config.GPIO_OType = GPIO_OType_PP;
			LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
			LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOB, &LED_Config);
		}

		virtual void set_LED(HELIOS_LED helios_led, LED_STATE state){
			uint8_t LED = (uint8_t)helios_led;

			if(state == LED_ON){
				GPIO_WriteBit(ledLUT[LED].port, ledLUT[LED].pin, Bit_SET);
			}else{
				GPIO_WriteBit(ledLUT[LED].port, ledLUT[LED].pin, Bit_RESET);
			}
		};

		virtual LED_STATE get_LED(HELIOS_LED helios_led){
			uint8_t LED = (uint8_t)helios_led;
			if(GPIO_ReadOutputDataBit(ledLUT[LED].port, ledLUT[LED].pin))
				return LED_ON;
			else return LED_OFF;
		};

	private:
		 LED_LUTentry ledLUT[6] = {
				/*ID*/				/*PORT*/		/*PIN*/
				{STATUS_LED_RED,	 GPIOB, 		GPIO_Pin_13},
				{STATUS_LED_GREEN,	 GPIOB,			GPIO_Pin_12},
				{DEBUG_RED,			 GPIOE,			GPIO_Pin_11},
				{DEBUG_YELLOW,		 GPIOE,			GPIO_Pin_8},
				{DEBUG_BLUE1,		 GPIOE, 		GPIO_Pin_7},
				{DEBUG_BLUE2,		 GPIOC, 		GPIO_Pin_5}
		};

};

