#pragma once

#include "Framework.hpp"


class FlightDynamics : public ApplicationModule{
	using ApplicationModule::ApplicationModule;
	public:


		HAL_Accelerometer_I* accelerometer;
		void set_accelerometer(HAL_Accelerometer_I* acc){accelerometer = acc;}


	protected:
		void init(void);
		void run(void);

	private:
		struct EEPROM_Structure{
			AccelerometerCalibration accelerometer_calibration;
		};

		EEPROM_Structure eeprom_structure;

		void handle_message(Message& msg);
};

