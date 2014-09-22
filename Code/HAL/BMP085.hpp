#pragma once

#include "Framework.hpp"
#include "GenericI2C.hpp"

#define P_SEALEVEL 101325.0f //101297
#define NUM_OF_OFFSET_CALCULATIONS 10.0f

class BMP085 {
	public:
		BMP085(void);
		~BMP085(void);
		bool		dataAvailable(void);
		float 		getAltitude(void);
		bool  		getStatus(void);
		void 		setOffset(float);

	private:
		float	 	getTemperature(void);
		bool		process(void);
		void 		calculateAltitude(void);
		void 		startTempConvertion(void);
		bool	 	readRawTemperature(void);
		void 		readCompTemperature(void);
		void		startPressConvertion(void);
		bool		readRawPressure(void);
		void 		readCompPressure(void);
		void		calculateAltitudeOffset(void);
		void		readCalibrationData(void);
		bool 		read16BitData(uint8_t addr, uint16_t* output);

		typedef struct {
			int16_t 	ac1;
			int16_t 	ac2;
			int16_t 	ac3;
			uint16_t 	ac4;
			uint16_t 	ac5;
			uint16_t 	ac6;
			int16_t 	b1;
			int16_t 	b2;
			int16_t 	mb;
			int16_t 	mc;
			int16_t 	md;
		} BMP085_CalibrationData;
		BMP085_CalibrationData calCoeff;

		typedef enum {
			BAROSTATE_CalculationDone,
			BAROSTATE_ReadTemp,
			BAROSTATE_ReadPressure,
			BAROSTATE_Calculate
		}BAROSTATE;
		BAROSTATE m_State;

		GenericI2C m_I2C;

		uint32_t m_TemperatureTimestamp;
		uint32_t m_PressureTimestamp;

		int32_t  m_RawTemperature;
		int32_t  m_RawPressure;
		int32_t  m_Pressure;
		float    m_Temperature;
		float    m_Altitude;
		float    m_PressureDebug;
		float 	 m_AltitudeOffset;
		uint8_t  m_Count;
		bool  	 m_ConvertionInProgress;
		bool	 m_CalibrationInProgress;
		bool 	 m_DataReady;

		float m_ReadRawPressureErrors1;
		float m_ReadRawPressureErrors2;
		float m_ReadRawTempErrors;
		float m_StartTempErrors;
		float m_StartPressErrors;


		//Pressure Sensor BMP085 device address
		static const uint8_t BMP085_ADDRESS 	= 0b1110111;
		static const uint8_t BMP085_DEV_ADDR_R 	= 0xEF;		//I2C address to read data
		static const uint8_t BMP085_DEV_ADDR_W 	= 0xEE;		//I2C address to write data

		//Control register address
		static const uint8_t BMP085_CTRL_REG_ADDR = 0xF4;	//Basic control register address
		static const uint8_t BMP085_CTRL_REG_TEMP = 0x2E;	//Control register address for temperature

		static const uint8_t BMP085_CTRL_REG_PRESS_0 =	0x34; //Control register address for oversampling mode 0
		static const uint8_t BMP085_CTRL_REG_PRESS_1 =	0x74; //Control register address for oversampling mode 1
		static const uint8_t BMP085_CTRL_REG_PRESS_2 =	0xB4; //Control register address for oversampling mode 2
		static const uint8_t BMP085_CTRL_REG_PRESS_3 =	0xF4; //Control register address for oversampling mode 3

		//Data register
		static const uint8_t BMP085_DATA_REG_N = 0xF6;		//Main data register address
		static const uint8_t BMP085_DATA_REG_X = 0xF8;		//Extended data register address for 19bit * resolution

		//EEPROM Register addresses for calibration data
		static const uint8_t BMP085_AC1_ADDR = 0xAA;
		static const uint8_t BMP085_AC2_ADDR = 0xAC;
		static const uint8_t BMP085_AC3_ADDR = 0xAE;
		static const uint8_t BMP085_AC4_ADDR = 0xB0;
		static const uint8_t BMP085_AC5_ADDR = 0xB2;
		static const uint8_t BMP085_AC6_ADDR = 0xB4;
		static const uint8_t BMP085_B1_ADDR  = 0xB6;
		static const uint8_t BMP085_B2_ADDR  = 0xB8;
		static const uint8_t BMP085_MB_ADDR  = 0xBA;
		static const uint8_t BMP085_MC_ADDR  = 0xBC;
		static const uint8_t BMP085_MD_ADDR  = 0xBE;

		static const uint8_t BMP085_MODE_ULTRALOWPOWER 	= 0;
		static const uint8_t BMP085_MODE_STANDARD 		= 1;
		static const uint8_t BMP085_MODE_HIGHRES  		= 2;
		static const uint8_t BMP085_MODE_ULTRAHIGHRES 	= 3;

		//Remember to change the control register data, in "StartPressConvertion(), if changing mode
		//Also remember to change the wait time in readRawPressure(), changing mode (5, 8, 14, 26)
		static const uint8_t MODE 	= BMP085_MODE_ULTRAHIGHRES;

};
