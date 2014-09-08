#pragma once

#include "Framework.hpp"


class FlightDynamics : public ApplicationModule{
	public:
		FlightDynamics(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0);
		void set_accelerometer(HAL_Accelerometer_I* acc){accelerometer = acc;}

	protected:
		void run(void);

	private:
		HAL_Accelerometer_I* accelerometer;

		struct EEPROM_Structure{
			AccelerometerCalibration accelerometer_calibration;
		};

		EEPROM_Structure eeprom_structure;

		void handle_message(Message& msg);
};

