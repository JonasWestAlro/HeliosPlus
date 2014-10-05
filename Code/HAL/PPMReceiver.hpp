#pragma once

#include "Framework.hpp"
#include <cstring>

#define PLM_RECEIVER_MAX_PULSE 2400
#define PLM_RECEIVER_MIN_PULSE 800

class PPMReceiver : public HAL_ControlReceiver_I{
	public:
		PPMReceiver();

		virtual void execute(void);

		virtual uint8_t  get_status(void);
		virtual float 	 get_channel(ControlReceiverChannel);
		virtual void 	 get_all_channels(ControlReceiverValues&);
		virtual void 	 start_calibration(void);
		virtual void 	 stop_calibration(void);
		virtual void 	 get_calibration(ControlReceiverCalibration*);
		virtual void 	 set_calibration(ControlReceiverCalibration*);

	private:
		void update_status();
		void calibrate();
		float get_normalized_channel(uint8_t);

		GenericPulseMeasurement channel1;
		GenericPulseMeasurement channel2;
		GenericPulseMeasurement channel3;
		GenericPulseMeasurement channel4;
		GenericPulseMeasurement channel5;
		GenericPulseMeasurement channel6;

		ControlReceiverCalibration calibration;
		ControlReceiverCalibration temp_calibration;

		ControlReceiverChannel channel_mapping[6];
		uint16_t 	channel_values[6] = {1500, 1500, 1500, 1500, 1500, 1500};
		ControlReceiverValues sorted_channel_values;

		bool 		calibrating = false;
		uint32_t 	last_update_timestamp[6] = {0};
		uint8_t 	status = STATUS_OK;

		GenericPulseMeasurement* channel_table[6] = {
				&channel1,
				&channel2,
				&channel3,
				&channel4,
				&channel5,
				&channel6,
		};

};
