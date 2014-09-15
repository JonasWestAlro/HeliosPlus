#pragma once

#include "Framework.hpp"

typedef enum{
	CHANNEL_THROTTLE,
	CHANNEL_PITCH,
	CHANNEL_ROLL,
	CHANNEL_YAW,
	CHANNEL_SWITCH,
	CHANNEL_CLICK,
}ControlReceiverChannel;

typedef struct{
	uint16_t high[6];
	uint16_t low[6];
}ControlReceiverCalibration;

class HAL_ControlReceiver_I : public HAL_Interface {
	public:
		virtual void     execute(void) = 0;
		virtual uint8_t  get_status(void) = 0;
		virtual float 	 get_channel(ControlReceiverChannel) = 0;
		virtual void 	 start_calibration(void) = 0;
		virtual void 	 stop_calibration(void) = 0;
		virtual void 	 get_calibration(ControlReceiverCalibration*) = 0;
		virtual void 	 set_calibration(ControlReceiverCalibration*) = 0;
};
