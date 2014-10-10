#include "GenericUART.hpp"
#include <stdio.h>

GenericUART::GenericUART(){};

GenericUART::GenericUART(GENERIC_UART_TYPE UART, uint32_t BaudRate)
: m_NextRXByteToApplication(0), m_TransmitBufferCounter(0), m_TransmitInProgress(0), m_ActiveTransmitBuffer(1)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	// GPIO Common
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

    // GPIO Setup
	switch (UART) {
		case GENERIC_UART_TYPE_USART1_PA9_PA10: // (TX A9, RX A10)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
			break;

		case GENERIC_UART_TYPE_USART1_PB6_PB7: // (TX B6, RX B7)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
			break;

		case GENERIC_UART_TYPE_USART2_PA2_PA3: // (TX A2, RX A3)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
			break;

		case GENERIC_UART_TYPE_USART2_PD5_PD6: // (TX D5, RX D6)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
			GPIO_Init(GPIOD, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
		    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
		    break;

		case GENERIC_UART_TYPE_USART3_PB10_PB11: // (TX B10, RX B11)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
		    break;

		case GENERIC_UART_TYPE_USART3_PC10_PC11: // (TX C10, RX C11)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
			GPIO_Init(GPIOC, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
		    break;

		case GENERIC_UART_TYPE_USART3_PD8_PD9: // (TX D8, RX D9)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
			GPIO_Init(GPIOD, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
		    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
		    break;

		case GENERIC_UART_TYPE_UART4_PA0_PA1: // (TX A0, RX A1)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
		    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
			break;

		case GENERIC_UART_TYPE_UART4_PC10_PC11: // (TX C10, RX C11)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
			GPIO_Init(GPIOC, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
		    break;

		case GENERIC_UART_TYPE_UART5_PC12_PD2: // (TX C12, RX D2)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
			GPIO_Init(GPIOC, &GPIO_InitStruct);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
			GPIO_Init(GPIOD, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
		    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
			break;

		case GENERIC_UART_TYPE_USART6_PC6_PC7: // (TX C6, RX C7)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_Init(GPIOC, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
		    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
			break;

		case GENERIC_UART_TYPE_USART6_PG9_PG14: // (TX G9, RX G14)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
			GPIO_Init(GPIOG, &GPIO_InitStruct);
		    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
		    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);
		    break;

		default:
			break;
	}

	// UART Common
    USART_InitStruct.USART_BaudRate = BaudRate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    switch (UART) {
		case GENERIC_UART_TYPE_USART1_PA9_PA10:
		case GENERIC_UART_TYPE_USART1_PB6_PB7:
			m_USART = USART1;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART2_PA2_PA3:
		case GENERIC_UART_TYPE_USART2_PD5_PD6:
			m_USART = USART2;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART3_PB10_PB11:
		case GENERIC_UART_TYPE_USART3_PC10_PC11:
		case GENERIC_UART_TYPE_USART3_PD8_PD9:
			m_USART = USART3;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		case GENERIC_UART_TYPE_UART4_PA0_PA1:
		case GENERIC_UART_TYPE_UART4_PC10_PC11:
			m_USART = UART4;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		case GENERIC_UART_TYPE_UART5_PC12_PD2:
			m_USART = UART5;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART6_PC6_PC7:
		case GENERIC_UART_TYPE_USART6_PG9_PG14:
			m_USART = USART6;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
			USART_Init(m_USART, &USART_InitStruct);
			USART_Cmd(m_USART, ENABLE);
			break;

		default:
			break;
	}

    // DMA RX Common
    m_DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)m_ReceiveBuffer;
    m_DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    m_DMA_InitStruct.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    m_DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    m_DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    m_DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    m_DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    m_DMA_InitStruct.DMA_BufferSize = RXBUFFERSIZE;
    m_DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    m_DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    m_DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    m_DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    m_DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

	// DMA RX Setup
	switch (UART) {
		case GENERIC_UART_TYPE_USART1_PA9_PA10:
		case GENERIC_UART_TYPE_USART1_PB6_PB7:
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
			m_DMA_InitStruct.DMA_Channel = DMA_Channel_4;
			m_DMA_InitStruct.DMA_PeripheralBaseAddr = USART1_BASE + 0x04;
			m_DMA_RX_Stream = DMA2_Stream2;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART2_PA2_PA3:
		case GENERIC_UART_TYPE_USART2_PD5_PD6:
		    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			m_DMA_InitStruct.DMA_Channel = DMA_Channel_4;
			m_DMA_InitStruct.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;
			m_DMA_RX_Stream = DMA1_Stream5;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART3_PB10_PB11:
		case GENERIC_UART_TYPE_USART3_PC10_PC11:
		case GENERIC_UART_TYPE_USART3_PD8_PD9:
		    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			m_DMA_InitStruct.DMA_Channel = DMA_Channel_4;
			m_DMA_InitStruct.DMA_PeripheralBaseAddr = USART3_BASE + 0x04;
			m_DMA_RX_Stream = DMA1_Stream1;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
		    break;

		case GENERIC_UART_TYPE_UART4_PA0_PA1:
		case GENERIC_UART_TYPE_UART4_PC10_PC11:
		    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
			m_DMA_InitStruct.DMA_Channel = DMA_Channel_4;
			m_DMA_InitStruct.DMA_PeripheralBaseAddr = UART4_BASE + 0x04;
			m_DMA_RX_Stream = DMA1_Stream2;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
		    break;

		case GENERIC_UART_TYPE_UART5_PC12_PD2:
		    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
		    m_DMA_InitStruct.DMA_Channel = DMA_Channel_4;
		    m_DMA_InitStruct.DMA_PeripheralBaseAddr = UART5_BASE + 0x04;
			m_DMA_RX_Stream = DMA1_Stream0;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);
		    break;

		case GENERIC_UART_TYPE_USART6_PC6_PC7:
		case GENERIC_UART_TYPE_USART6_PG9_PG14:
		    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		    m_DMA_InitStruct.DMA_Channel = DMA_Channel_5;
		    m_DMA_InitStruct.DMA_PeripheralBaseAddr = USART6_BASE + 0x04;
			m_DMA_RX_Stream = DMA2_Stream1;
			DMA_Init(m_DMA_RX_Stream, &m_DMA_InitStruct);
		    DMA_Cmd(m_DMA_RX_Stream, ENABLE);
		    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);
		    break;

		default:
			break;
	}

	// DMA TX Common
    m_DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)m_TransmitBuffer_1;
    m_DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    m_DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    m_DMA_InitStruct.DMA_Priority = DMA_Priority_High;

	// DMA TX Setup
	switch (UART) {
		case GENERIC_UART_TYPE_USART1_PA9_PA10:
		case GENERIC_UART_TYPE_USART1_PB6_PB7:
			m_DMA_TX_Stream = DMA2_Stream5;
			m_DMA_TCIF = DMA_FLAG_TCIF5;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART2_PA2_PA3:
		case GENERIC_UART_TYPE_USART2_PD5_PD6:
			m_DMA_TX_Stream = DMA1_Stream6;
			m_DMA_TCIF = DMA_FLAG_TCIF6;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
			break;

		case GENERIC_UART_TYPE_USART3_PB10_PB11:
		case GENERIC_UART_TYPE_USART3_PC10_PC11:
		case GENERIC_UART_TYPE_USART3_PD8_PD9:
			m_DMA_TX_Stream = DMA1_Stream3;
			m_DMA_TCIF = DMA_FLAG_TCIF3;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
			break;

		case GENERIC_UART_TYPE_UART4_PA0_PA1:
		case GENERIC_UART_TYPE_UART4_PC10_PC11:
			m_DMA_TX_Stream = DMA1_Stream4;
			m_DMA_TCIF = DMA_FLAG_TCIF4;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
		    break;

		case GENERIC_UART_TYPE_UART5_PC12_PD2:
			m_DMA_TX_Stream = DMA1_Stream7;
			m_DMA_TCIF = DMA_FLAG_TCIF7;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
		    break;

		case GENERIC_UART_TYPE_USART6_PC6_PC7:
		case GENERIC_UART_TYPE_USART6_PG9_PG14:
			m_DMA_TX_Stream = DMA2_Stream6;
			m_DMA_TCIF = DMA_FLAG_TCIF6;
			DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);
			USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
		    break;

		default:
			break;
	}
}

/**
  * @brief  	Get the number of bytes that has been received
  * @param[in]  uart 	The uart device to use
  * @return		The number of bytes available
  */
uint16_t GenericUART::data_available(void)
{
	uint16_t temp = DMA_GetCurrDataCounter(m_DMA_RX_Stream);
	return RXBUFFERSIZE - temp - m_NextRXByteToApplication;
}


/** @brief  	Send multiple bytes of data to the TX buffer.
  * 			Call #GenUART_Transmit to transmit data
  * @param[in]  uart 	The uart device to use
  * @param[in]  data 	Pointer to the data
  * @param[in]  size 	Number of bytes to send
  * @retval 	0		Failure
  * @retval		1		Success
  */
uint16_t GenericUART::put(char* data, uint16_t size)
{
   uint16_t i;
	for(i=0; i<size; i++){
		if(!this->send(data[i])) return false;
	}
	return true;

}

uint16_t GenericUART::put(char* data){
	uint8_t i = 0;

	while(data[i] != '\0'){
		send(data[i++]);
	}

	return i;
}

/** @brief  	Send one byte of data to the TX buffer.
  * 			Call #GenUART_Transmit to transmit data
  * @param[in]  uart 	The uart device to use
  * @param[in]  data 	Data to send
  * @retval 	0		Failure
  * @retval		1		Success
  */
bool GenericUART::send(uint8_t data){
	if(m_ActiveTransmitBuffer == 1){
		if(m_TransmitBufferCounter >= RXBUFFERSIZE-1) return false;
		m_TransmitBuffer_1[m_TransmitBufferCounter++] = data;
	}else{
		if(m_TransmitBufferCounter >= RXBUFFERSIZE-1) return false;
		m_TransmitBuffer_2[m_TransmitBufferCounter++] = data;
	}

	return true;
}

bool GenericUART::send_number(uint32_t number){
    uint8_t i = 0;
	char data[10] = {0};
    uint8_t len = snprintf(data, 10, "%d", (int)number);

    for(i=0; i<len; i++){
    	if(!this->send(data[i])) return false;
    }
    return true;
}

bool GenericUART::send_number(float number){
    uint8_t i = 0;
	char data[10] = {0};
    uint8_t len = snprintf(data, 10, "%f", number);

    for(i=0; i<len; i++){
    	if(!this->send(data[i])) return false;
    }
    return true;
}

/** @brief  	Transmit the data that has been placed in the TX buffer
  * @param[in]  uart 	The uart device to use
  * @retval 	0		Failure
  * @retval		1		Success
  */
uint16_t GenericUART::transmit(void)
{
	//Check if there are anything in the buffer:
	if(m_TransmitBufferCounter > 0){
		//Check if UART is busy, report error if it is:
		if(this->isBusy()){
			return false;
		}

		//Setup DMA Transfer, and switch the buffer:
		if(m_ActiveTransmitBuffer == 1){
			m_DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)m_TransmitBuffer_1;
			m_ActiveTransmitBuffer = 2;
		}else{
			m_DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)m_TransmitBuffer_2;
			m_ActiveTransmitBuffer = 1;
		}

		m_DMA_InitStruct.DMA_BufferSize = m_TransmitBufferCounter;
		DMA_Init(m_DMA_TX_Stream, &m_DMA_InitStruct);

		//Reset DMA done flag:
		DMA_ClearFlag(m_DMA_TX_Stream, m_DMA_TCIF);

		//Trig the UART
		USART_ClearFlag(m_USART, USART_FLAG_TC);

		//START DMA TRANSFER
		DMA_Cmd(m_DMA_TX_Stream, ENABLE);

		//Reset the counter
		m_TransmitBufferCounter = 0;
		return true;
	}
	return true;
}


/** @brief  	Read one byte of data from the UART
  * @param[in]  data 	Pointer where data should be read to
  */
bool GenericUART::receive(uint8_t* data){
	if(this->data_available()){
		//Get the next byte for the application:
		*data = m_ReceiveBuffer[m_NextRXByteToApplication];
		//Increment counter:
		m_NextRXByteToApplication++;
		m_NextRXByteToApplication %= RXBUFFERSIZE;
		return true;
	}
	return false;
}

/** @brief  	Get the state of the UART
  * @retval		0		UART is NOT busy
  * @retval		1		UART IS busy
  */
bool GenericUART::isBusy(void)
{
	if (USART_GetFlagStatus(m_USART, USART_FLAG_TC) == RESET) {
		return true;
	}else {
		return false;
	}
}
