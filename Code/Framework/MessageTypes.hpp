#pragma once
/*! \file */


//! MessageType
typedef enum{
	MESSAGE_INVALID = 0,
	REQUEST_SENSORS_REPORT,			//!< Is used to request all sensors to report in
	REQUEST_MOTORS_REPORT,			//!< Any module handling motors should report in functional
	REQUEST_CONTROLINPUTS_REPORT,	//!< Any module handling controls should report in functional
	REQUEST_SHIFT_OF_CONTROL,		//!< Used to shift control of drone to a new control input
	SHIFT_OF_CONTROL_ACK,			//!< Used to notify an Input module about control!
	SENSOR_REPORT_STATUS,			//!< Is used to report sensor status
	MOTOR_REPORT_STATUS,			//!< This is used by motors handling modules to report their status
	CONTROLINPUT_REPORT_STATUS,		//!< This should be used be modules providing control interfaces to report their status
	ARM_REQUEST,					//!< Is used to request the system to arm it self
	UNARM_REQUEST,					//!<
	CALIBRATE_GYROSCOPE,			//!<
	CALIBRATE_ACCELEROMETER,		//!<
	CALIBRATE_MAGNETOMETER,			//!<
	CALIBRATE_CONTROLINPUT,			//!<
	START_DEBUG_STREAM,				//!<
	STOP_DEBUG_STREAM				//!<
	/*Define more messages here for
	 * your application*/
}MessageType;

/*********** Application Message Descriptions ************************
 *
 * REQUEST_SENSORS_REPORT
 * 		(Is used to request all sensors to report in)
 *
 * REQUEST_MOTORS_REPORT
 * 		(Any module handling motors should report in functional)
 *
 * REQUEST_CONTROLS_REPORT
 * 		(Any module handling controls should report in functional)
 *
 * REQUEST_SHIFT_OF_CONTROL
 * 		(Used to shift control of drone to a new control input)
 * 		Message[0]: 1 = TAKE CONTROL .. 2 = LEAVE CONTROL
 * 		Ptr = Module_Desc* to new ControlInput
 */
	enum{REQUEST_TAKE_CONTROL = 0,
		 REQUEST_LEAVE_CONTROL = 1};

 /*
 * SHIFT_OF_CONTROL_ACK
 * 		(Used to notify an Input module about control!)
 * 		Message[0]: 1 = TAKE CONTROL .. 0 = LEAVE CONTROL
 * 		Message[1]: 1 = ACK .. 0 = NACK
 */
	enum{ACK = 0,
		 NACK = 1};

/*
 * SENSOR_REPORT_STATUS
 * 		(Is used to report sensor status)
 * 		uint32_t Data = MAV_SYS_STATUS_SENSOR (flags)
 *		uint8_t Data[5] = Status 0:NOTOK 1:OK 2:Calibrating
 */


/* MOTOR_REPORT_STATUS
 * 		(This is used by motors handling modules to report their status)
 *	 	Ptr 	 = Module_Desc* to ControlInput
 * 		uint8Msg = Status 0:NOTOK 1:OK 2:Calibrating
 *
 * CONTROLINPUT_REPORT_STATUS
 *		(This should be used be modules providing control interfaces to
 *		 report their status).
 *		 uint8Msg = Status 0:NOTOK 1:OK 2:Calibrating
 *
 * ARM_REQUEST
 * 		(Is used to request the system to arm it self)
 *
 * UNARM_REQUEST
 *
 *
 * */
