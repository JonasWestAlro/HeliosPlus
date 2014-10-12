#pragma once
/*! \file */


//! MessageType
typedef enum{
	MESSAGE_INVALID = 0,
	REQUEST_FLIGHTDYNAMICS_REPORT,	//!< Is used to request all sensors to report in
	REQUEST_MOTORS_REPORT,			//!< Any module handling motors should report in functional
	REQUEST_CONTROLINPUTS_REPORT,	//!< Any module handling controls should report in functional
	REQUEST_SHIFT_OF_CONTROL,		//!< Used to shift control of drone to a new control input
	SHIFT_OF_CONTROL_ACK,			//!< Used to notify an Input module about control!
	FLIGHTDYNAMICS_REPORT_STATUS,			//!< Is used to report sensor status
	MOTOR_REPORT_STATUS,			//!< This is used by motors handling modules to report their status
	CONTROLINPUT_REPORT_STATUS,		//!< This should be used be modules providing control interfaces to report their status
	ARM_REQUEST,					//!< Is used to request the system to arm it self
	UNARM_REQUEST,					//!<
	CALIBRATE_GYROSCOPE,			//!<
	CALIBRATE_ACCELEROMETER,		//!<
	CALIBRATE_MAGNETOMETER,			//!<
	CALIBRATE_CONTROLINPUT,			//!<
	START_DEBUG_STREAM,				//!<
	STOP_DEBUG_STREAM,				//!<
	CLI_PRINT_REQUEST,				//!<
	CLI_ACK_PRINT
	/*Define more messages here for
	 * your application*/
}MessageType;

/*********** Application Message Descriptions ************************
 *
 * REQUEST_FLIGHTDYNAMICS_REPORT
 * 		(Is used to request the module handling flight dynamics to report in)
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
 * REQUEST_FLIGHTDYNAMICS_REPORT
 * 		(Is used by module(s) handling flight dynamics to report its status)
 */


/* MOTOR_REPORT_STATUS
 * 		(This is used by motors handling modules to report their status)
 *	 	Ptr 	 = Module_Desc* to ControlInput
 * 		uint8Msg = Status 0:NOTOK 1:OK 2:Calibrating
 *
 * CONTROLINPUT_REPORT_STATUS
 *		(This should be used be modules providing control interfaces to
 *		 report their status).
/*/
	enum{
		MANUAL_CONTROL_INPUT = 0,
		NON_MANUAL_CONTROL_INPUT
	};
/*
 * ARM_REQUEST
 * 		(Is used to request the system to arm it self)
 *
 * UNARM_REQUEST
 *
 * CLI_REQUEST_PRINT
 * 		(Is used by the CLI to request a module to print something on the Debug channel)
 *
 * CLI_ACK_PRINT
 *		(Is used by modules to report to CLI that they printed something to the Debug channel)
 *
 * */
