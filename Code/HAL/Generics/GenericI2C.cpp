#include "GenericI2C.hpp"

GenericI2C::GenericI2C(GENERIC_I2C_TYPE I2C, uint32_t ClockSpeed)
{
	m_Type = I2C;
	m_ClockSpeed = ClockSpeed;

	this->setup();
}

bool GenericI2C::read8Register(uint8_t address, uint8_t registerAddress, uint8_t n, uint8_t* data)
{
	//Start a transmission in Master transmitter mode
	if (!this->start(address<<1, I2C_Direction_Transmitter)) return false;

	//Send the register address
	if (!this->write(registerAddress)) return false;

	//Start a transmission in Master receiver mode
	if (!this->start(address<<1, I2C_Direction_Receiver)) return false;

	uint8_t i;
	for(i=0; i<n-1; i++){
		//Read one byte and request another byte
		if (!this->readAck(&(data[i]))) return false;
	}

	//Read one byte and don't request another byte, stop transmission
	if (!this->readNack(&(data[n-1]))) return false;

	I2C_GenerateSTOP(m_I2C, ENABLE);

	return true;
}

bool GenericI2C::read16Register(uint8_t address, uint16_t registerAddress, uint8_t n, uint8_t* data)
{
	//Start a transmission in Master transmitter mode
	if (!this->start(address<<1, I2C_Direction_Transmitter)) return false;

	//Send the highest register address byte
	if (!this->write(registerAddress >> 8)) return false;

	//Send the lowest register address byte
	if (!this->write(registerAddress & 0x00FF)) return false;

	//Start a transmission in Master receiver mode
	if (!this->start(address<<1, I2C_Direction_Receiver)) return false;

	uint8_t i;
	for(i=0; i<n-1; i++){
		//Read one byte and request another byte
		if (!this->readAck(&(data[i]))) return false;
	}

	//Read one byte and don't request another byte, stop transmission
	if (!this->readNack(&(data[n-1]))) return false;

	I2C_GenerateSTOP(m_I2C, ENABLE);

	return true;
}

bool GenericI2C::write8Register(uint8_t address, uint8_t registerAddress, uint8_t n, uint8_t* data)
{
	//Start a transmission in Master transmitter mode
	if (!this->start(address<<1, I2C_Direction_Transmitter)) return false;

	//Send the register address
	if (!this->write(registerAddress)) return false;

	//Write all bytes:
	uint8_t i;
	for(i=0; i<n; i++){
		//Read one byte and request another byte
		if (!this->write(data[i])) return false;
	}

	I2C_GenerateSTOP(m_I2C, ENABLE);

	return 1;
}

bool GenericI2C::write16Register(uint8_t address, uint16_t registerAddress, uint8_t n, uint8_t* data)
{
	//Start a transmission in Master transmitter mode
	if (!this->start(address<<1, I2C_Direction_Transmitter)) return false;

	//Send the highest register address byte
	if (!this->write(registerAddress >> 8)) return false;

	//Send the lowest register address byte
	if (!this->write(registerAddress & 0x00FF)) return false;

	//Write all bytes:
	uint8_t i;
	for(i=0; i<n; i++){
		//Read one byte and request another byte
		if (!this->write(data[i])) return false;
	}

	I2C_GenerateSTOP(m_I2C, ENABLE);

	return true;
}

void GenericI2C::restart(void)
{
	I2C_GenerateSTOP(m_I2C, ENABLE);
	I2C_DeInit(m_I2C);
	this->setup();
}


void GenericI2C::setup(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// Common GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	switch (m_Type) {
		case GENERIC_I2C_TYPE_I2C1_PB6_PB7: // SCL PB6 - SDA PB7
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
			break;

		case GENERIC_I2C_TYPE_I2C1_PB8_PB9: // SCL PB8 - SDA PB9
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
			break;

		case GENERIC_I2C_TYPE_I2C2_PF1_PF0: // SCL PF1 - SDA PF0
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
			GPIO_Init(GPIOF, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);
			break;

		case GENERIC_I2C_TYPE_I2C2_PH4_PH5: // SCL PH4 - SDA PH5
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_Init(GPIOH, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOH, GPIO_PinSource4, GPIO_AF_I2C2);
			GPIO_PinAFConfig(GPIOH, GPIO_PinSource5, GPIO_AF_I2C2);
			break;

		case GENERIC_I2C_TYPE_I2C3_PH7_PH8: // SCL PH7 - SDA PH8
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
			GPIO_Init(GPIOH, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_I2C3);
			GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_I2C3);
			break;
		case GENERIC_I2C_TYPE_I2C3_PA8_PC9: // SCL PA8 - SDA PC9
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
			GPIO_Init(GPIOC, &GPIO_InitStruct);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
			GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);
			break;

		default:
			break;
	}

	// I2C Common
    I2C_InitStruct.I2C_ClockSpeed = m_ClockSpeed;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	switch (m_Type) {
		case GENERIC_I2C_TYPE_I2C1_PB6_PB7:
		case GENERIC_I2C_TYPE_I2C1_PB8_PB9:
		    I2C_Init(I2C1, &I2C_InitStruct);
		    I2C_Cmd(I2C1, ENABLE);
			break;

		case GENERIC_I2C_TYPE_I2C2_PF1_PF0:
		case GENERIC_I2C_TYPE_I2C2_PH4_PH5:
		    I2C_Init(I2C2, &I2C_InitStruct);
		    I2C_Cmd(I2C2, ENABLE);
			break;

		case GENERIC_I2C_TYPE_I2C3_PH7_PH8:
		case GENERIC_I2C_TYPE_I2C3_PA8_PC9:
		    I2C_Init(I2C3, &I2C_InitStruct);
		    I2C_Cmd(I2C3, ENABLE);
			break;

		default:
			break;
	}
}

bool GenericI2C::start(uint8_t address, uint8_t direction)
{
	// Send I2C START condition
	I2C_GenerateSTART(m_I2C, ENABLE);

	// wait for I2C EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_MODE_SELECT));
		// Need to check for timeout (Remove semicolon)
		//CHECK_TIMEOUT(TIMEOUT);

	// Send slave Address for write
	I2C_Send7bitAddress(m_I2C, address, direction);

	/* wait for I2C EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
			// Need to check for timeout (Remove semicolon)
			//CHECK_TIMEOUT(TIMEOUT);
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
			// Need to check for timeout (Remove semicolon)
			//CHECK_TIMEOUT(TIMEOUT);
	}
	else {
		return false;
	}

	return true;
}

bool GenericI2C::write(uint8_t data)
{
	I2C_SendData(m_I2C, data);

	while(!I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		// Need to check for timeout (Remove semicolon)
		//CHECK_TIMEOUT(TIMEOUT);

	return true;
}

bool GenericI2C::readAck(uint8_t* data)
{
	// Enable acknowledge of recieved data
	I2C_AcknowledgeConfig(m_I2C, ENABLE);
	// Wait until one byte has been received
	while( !I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED));
		// Need to check for timeout (Remove semicolon)
		//CHECK_TIMEOUT(TIMEOUT);

	// Read data from I2C data register and return data byte
	*data = I2C_ReceiveData(m_I2C);

	return true;
}

bool GenericI2C::readNack(uint8_t* data)
{
	// disabe acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(m_I2C, DISABLE);
	I2C_GenerateSTOP(m_I2C, ENABLE);

	// Wait until one byte has been received
	while(!I2C_CheckEvent(m_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED));
		// Need to check for timeout (Remove semicolon)
		//CHECK_TIMEOUT(TIMEOUT);

	// Read data from I2C data register and return data byte
	*data = I2C_ReceiveData(m_I2C);

	return true;
}
