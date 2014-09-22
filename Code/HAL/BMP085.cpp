#include "BMP085.hpp"
#include "math.h"

BMP085::BMP085(void)
: m_I2C(GENERIC_I2C_TYPE_I2C1_PB6_PB7, 100000)
{
	m_TemperatureTimestamp = 0;
	m_PressureTimestamp = 0;
	m_RawTemperature = 0;
	m_RawPressure = 0;
	m_Pressure = 0;
	m_Temperature = 0;
	m_Altitude = 0;
	m_PressureDebug = 0;
	m_AltitudeOffset = 0;
	m_Count = 0;
	m_ConvertionInProgress = false;
	m_CalibrationInProgress = true;
	m_DataReady = false;
	m_ReadRawPressureErrors1 = 0;
	m_ReadRawPressureErrors2 = 0;
	m_ReadRawTempErrors = 0;
	m_StartTempErrors = 0;
	m_StartPressErrors = 0;
	m_State = BAROSTATE_ReadTemp;

	this->readCalibrationData();

	m_I2C.restart(400000);
}

BMP085::~BMP085(void)
{

}


bool BMP085::dataAvailable(void)
{
	this->process();
	return m_DataReady;
}

float BMP085::getAltitude(void)
{
	static float last_altitude;
	float dif = last_altitude - m_Altitude;

	if(!m_CalibrationInProgress)
		if(dif > 1000 || dif < -1000){
			return last_altitude;
		}else{
			return m_Altitude;
		}
	else return 0;
}

float BMP085::getTemperature(void)
{
	return m_Temperature;
}

bool BMP085::process(void)
{
	switch (m_State) {
		case BAROSTATE_ReadTemp:
			m_DataReady = false;
			if(m_ConvertionInProgress == false){
				m_ConvertionInProgress = true;
				this->startTempConvertion();
			}
			if(this->readRawTemperature() == true){
				m_State = BAROSTATE_ReadPressure;
			}
			break;

		case BAROSTATE_ReadPressure:
			m_DataReady = false;
			if(m_ConvertionInProgress == false){
				m_ConvertionInProgress = true;
				startPressConvertion();
			}
			if(this->readRawPressure() == true){
				m_State = BAROSTATE_Calculate;
			}
			break;

		case BAROSTATE_Calculate:
			m_DataReady = false;
			this->readCompTemperature();
			this->calculateAltitude();
			if(m_CalibrationInProgress == true){
				this->calculateAltitudeOffset();
			}
			m_State = BAROSTATE_CalculationDone;
			break;

		case BAROSTATE_CalculationDone:
			m_DataReady = true;
			m_State = BAROSTATE_ReadTemp;
			break;

		default:
			break;
	}

	return m_DataReady;
}


// Helper functions //
void BMP085::calculateAltitude(void)
{
	float newAltitude;
	this->readCompPressure();

	if(!m_CalibrationInProgress){
		newAltitude = 44330*(1-pow((m_Pressure/P_SEALEVEL),0.19)) - m_AltitudeOffset;
		m_Altitude = newAltitude;//0.9*Altitude + 0.1*newAltitude;
	}else{
		newAltitude = 44330*(1-pow((m_Pressure/P_SEALEVEL),0.19));
		m_Altitude = newAltitude;
	}

}


void BMP085::startTempConvertion(void)
{
	uint8_t data = BMP085_CTRL_REG_TEMP; // need to do this because we can't take the address of the static const variable
	m_TemperatureTimestamp = Time.get_timestamp();
	if(!m_I2C.write_register8(BMP085_ADDRESS, BMP085_CTRL_REG_ADDR, 1, &data))
	{
		m_StartTempErrors++;
		m_ConvertionInProgress = false;
	}
}

bool BMP085::readRawTemperature(void)
{
	uint32_t time = Time.get_time_since_ms(m_TemperatureTimestamp);

	if (time >= 5) {
		uint16_t temp;
		if (this->read16BitData(BMP085_DATA_REG_N, &temp) == false) {
			m_ReadRawTempErrors++;
			return false;
		}

		m_RawTemperature = temp;
		m_ConvertionInProgress = false;
		return true;
	}
	return false;
}

void BMP085::readCompTemperature(void)
{
	int32_t x1 = 0;
	int32_t x2 = 0;
	int32_t b5 = 0;
	int32_t compt = 0;

    x1 = ((int32_t) m_RawTemperature - (int32_t) calCoeff.ac6) * (int32_t) calCoeff.ac5 >> 15;
    x2 = ((int32_t) calCoeff.mc << 11) / (x1 + calCoeff.md);
    b5 = x1 + x2;
    compt = (b5 + 8) >> 4;
    m_Temperature = compt/10.0;
}


void BMP085::startPressConvertion(void)
{
	uint8_t data = BMP085_CTRL_REG_PRESS_3; // need to do this because we can't take the address of the static const variable
	if (m_I2C.write_register8(BMP085_ADDRESS, BMP085_CTRL_REG_ADDR, 1, &data) == false) {
		m_StartPressErrors++;
		m_ConvertionInProgress = false;
	}
}


bool BMP085::readRawPressure(void)
{
	uint32_t time = Time.get_time_since_ms(m_PressureTimestamp);

	if (time >= 26) {
		uint8_t  temp8;
		uint16_t temp16;
		int32_t pressure;

		if (this->read16BitData(BMP085_DATA_REG_N, &temp16) == false) {
			m_ReadRawPressureErrors1++;
			return false;
		}

		pressure = temp16;
		pressure = pressure << 8;

		Time.delay_us(20);

		if (m_I2C.read_register8(BMP085_ADDRESS, BMP085_DATA_REG_X, 1, &temp8) == false) {
			m_ReadRawPressureErrors2++;
			return false;
		}

		pressure += temp8;
		m_RawPressure =  (pressure >> (8 - MODE));

		m_ConvertionInProgress = false;
		return true;
	}
	return false;
}

void BMP085::setOffset(float offset)
{
	if(m_CalibrationInProgress == false)
		m_AltitudeOffset += offset;
}

void BMP085::readCompPressure(void)
{
	//int32_t compt = 0;
	int32_t ut = 0;
	int32_t up = 0;

	int32_t x1, x2, b5, b6, x3, b3, p;
	uint32_t b4, b7;

	ut = m_RawTemperature;
	up = m_RawPressure;

	x1 = ((int32_t) ut - (int32_t) calCoeff.ac6) * (int32_t) calCoeff.ac5 >> 15;
	x2 = ((int32_t) calCoeff.mc << 11) / (x1 + calCoeff.md);
	b5 = x1 + x2;
	//compt = (b5 + 8) >> 4;

	b6 = b5 - 4000;
	x1 = (calCoeff.b2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (calCoeff.ac2 * b6) >> 11;
	x3 = x1 + x2;
	b3 = (((((int32_t) calCoeff.ac1) * 4 + x3) << MODE) + 2) >> 2;
	x1 = (calCoeff.ac3 * b6) >> 13;
	x2 = (calCoeff.b1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (calCoeff.ac4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = ((uint32_t) (up - b3) * (50000 >> MODE));

	if (b7 < 0x80000000) {
		p = (b7 << 1) / b4;
	} else {
		p = (b7 / b4) << 1;
	}

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	m_Pressure = p + ((x1 + x2 + 3791) >> 4);
	m_PressureDebug = (float)m_Pressure;
}

float test[15];
void BMP085::calculateAltitudeOffset(void)
{
	if(m_Count >= NUM_OF_OFFSET_CALCULATIONS+1){
		m_CalibrationInProgress = false;
		m_Altitude = 0;
	}else{
		if(m_Count > 0){
			m_AltitudeOffset += (m_Altitude*(1/NUM_OF_OFFSET_CALCULATIONS));
		}
	}
	test[m_Count] = m_Altitude;
	m_Count++;
}

/*!
 * @brief	Read the calibration data from BMP085
 */
void BMP085::readCalibrationData(void)
{
	while(1){
		if( this->read16BitData(BMP085_AC1_ADDR, (uint16_t*)&calCoeff.ac1) &&
			this->read16BitData(BMP085_AC2_ADDR, (uint16_t*)&calCoeff.ac2) &&
			this->read16BitData(BMP085_AC3_ADDR, (uint16_t*)&calCoeff.ac3) &&
			this->read16BitData(BMP085_AC4_ADDR, (uint16_t*)&calCoeff.ac4) &&
			this->read16BitData(BMP085_AC5_ADDR, (uint16_t*)&calCoeff.ac5) &&
			this->read16BitData(BMP085_AC6_ADDR, (uint16_t*)&calCoeff.ac6) &&
			this->read16BitData(BMP085_B1_ADDR,  (uint16_t*)&calCoeff.b1)  &&
			this->read16BitData(BMP085_B2_ADDR,  (uint16_t*)&calCoeff.b2)  &&
			this->read16BitData(BMP085_MB_ADDR,  (uint16_t*)&calCoeff.mb)  &&
			this->read16BitData(BMP085_MC_ADDR,  (uint16_t*)&calCoeff.mc)  &&
			this->read16BitData(BMP085_MD_ADDR,  (uint16_t*)&calCoeff.md))
		{
			break;
		}else{
			m_I2C.restart();
		}
	}
}



bool BMP085::read16BitData(uint8_t addr, uint16_t* output)
{
	uint8_t Result[2];
	bool err;
	err = m_I2C.read_register8(BMP085_ADDRESS, addr, 2, Result);
    *output = Result[0]<<8 | Result[1];
    return err;
}

bool BMP085::getStatus(void)
{
	static uint32_t last_error = 0;

	uint32_t no_errors = 	m_ReadRawPressureErrors1	+
							m_ReadRawPressureErrors2	+
							m_ReadRawTempErrors 		+
							m_StartTempErrors 		+
							m_StartPressErrors;

	if(no_errors != last_error){
		return false;
	}

	return true;
}

