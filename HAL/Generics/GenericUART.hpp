#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"

#define RXBUFFERSIZE 2000

typedef enum{
	GENERIC_UART_TYPE_USART1_PA9_PA10 = 0,	/*!< TX PA9,  RX PA10 */
	GENERIC_UART_TYPE_USART1_PB6_PB7,		/*!< TX PB6,  RX PB7 */
	GENERIC_UART_TYPE_USART2_PA2_PA3,		/*!< TX PA2,  RX PA3 */
	GENERIC_UART_TYPE_USART2_PD5_PD6,		/*!< TX PD5,  RX PD6 */
	GENERIC_UART_TYPE_USART3_PB10_PB11,		/*!< TX PB10, RX PB11 */
	GENERIC_UART_TYPE_USART3_PC10_PC11,		/*!< TX PC10, RX PC11 */
	GENERIC_UART_TYPE_USART3_PD8_PD9,		/*!< TX PD8,  RX PD9 */
	GENERIC_UART_TYPE_UART4_PA0_PA1,		/*!< TX PA0,  RX PA1 */
	GENERIC_UART_TYPE_UART4_PC10_PC11,		/*!< TX PC10, RX PC11 */
	GENERIC_UART_TYPE_UART5_PC12_PD2,		/*!< TX PC12, RX PD2 */
	GENERIC_UART_TYPE_USART6_PC6_PC7,		/*!< TX PC6,  RX PC7 */
	GENERIC_UART_TYPE_USART6_PG9_PG14		/*!< TX PG9,  RX PG14 */
}GENERIC_UART_TYPE;

class GenericUART {
	public:
		GenericUART();
		GenericUART(GENERIC_UART_TYPE USART, uint32_t BaudRate);

		uint16_t dataAvailable(void);
		bool receive(uint8_t* data);
		bool send(uint8_t* data, uint16_t size);
		bool sendInteger(uint32_t number);
		bool sendFloat(float number);
		bool transmit(void);
		bool isBusy(void);

	private:
		bool put(uint8_t data);

		USART_TypeDef* m_USART;
		DMA_Stream_TypeDef* m_DMA_RX_Stream;
		DMA_Stream_TypeDef* m_DMA_TX_Stream;
		DMA_InitTypeDef m_DMA_InitStruct;

		uint32_t m_DMA_TCIF;
		uint8_t  m_ReceiveBuffer[RXBUFFERSIZE];
		uint8_t  m_TransmitBuffer_1[RXBUFFERSIZE];
		uint8_t  m_TransmitBuffer_2[RXBUFFERSIZE];
		uint16_t m_NextRXByteToApplication;
		uint16_t m_TransmitBufferCounter;
		uint8_t  m_TransmitInProgress;
		uint8_t  m_ActiveTransmitBuffer;
};
