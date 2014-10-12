#pragma once

#include "HAL_Interface.hpp"

typedef enum{
	CHANNEL_THROTTLE = 0,
	CHANNEL_PITCH,
	CHANNEL_ROLL,
	CHANNEL_YAW,
	CHANNEL_AUX1,
	CHANNEL_AUX2,
	CHANNEL_AUX3,
	CHANNEL_AUX4,
}ControlReceiverChannel;

struct ControlReceiverValues{
	float throttle = 0;
	float pitch = 0;
	float roll = 0;
	float yaw = 0;
	float aux1 = 0;
	float aux2 = 0;
	float aux3 = 0;
	float aux4 = 0;
};

typedef struct{
	uint16_t high[6];
	uint16_t low[6];
}ControlReceiverCalibration;

class HAL_ControlReceiver_I : public HAL_Interface {
	public:
		virtual void     execute(void) = 0;
		virtual uint8_t  get_status(void) = 0;
		virtual float 	 get_channel(ControlReceiverChannel) = 0;
		virtual void 	 get_all_channels(ControlReceiverValues&) = 0;
		virtual void 	 start_calibration(void) = 0;
		virtual void 	 stop_calibration(void) = 0;
		virtual void 	 get_calibration(ControlReceiverCalibration*) = 0;
		virtual void 	 set_calibration(ControlReceiverCalibration*) = 0;
};
