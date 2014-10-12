#pragma once
#include "Framework.hpp"

enum FlightConditionType{
	COND_FLIGHTDYNAMICS = 0, 	//Accelerometer has to be OK
	COND_MOTORS,				//Motors has to be OK
	COND_MANUAL_CONTROLINPUT,	//The manual joystick control that are connected to Motor module
								//has to be OK
	COND_ACTIVE_CONTROLINPUT	//The control that are connected to Motor module
								//has to be OK
};

struct FlightCondition{
	FlightConditionType condition;
	STATUS 			   	status;
	uint32_t 		   	timestamp;
};

static const uint8_t no_conditions = 5;

class ConditionsController{
public:
	void update_condition(FlightConditionType condition, STATUS status){
		conditions[condition].status = status;
		conditions[condition].timestamp = Time.get_timestamp();
	}

	STATUS get_condition(FlightConditionType condition){
		return conditions[condition].status;
	}

	uint32_t get_condition_timstamp(FlightConditionType condition){
		return conditions[condition].timestamp;
	}

	STATUS get_status(){
		for(uint8_t i=0; i<no_conditions; i++){
			if(conditions[i].status != STATUS_OK){
				//This one is NOT_OK! Check if we already are in a NOT_OK state:
				return conditions[i].status;
			}
		}

		return STATUS_OK;
	}

	uint8_t get_no_conditions_failed(){
		uint8_t result = 0;

		for(uint8_t i=0; i<no_conditions; i++){
			if(conditions[i].status != STATUS_OK){
				//This one is NOT_OK! Check if we already are in a NOT_OK state:
				result++;
			}
		}

		return result;
	}

private:
	FlightCondition conditions[5] = {
		/*   CONDITION        		OK  	TIMESTAMP*/
		{COND_FLIGHTDYNAMICS,   	  STATUS_NOTOK,    0},
		{COND_MOTORS, 		   		  STATUS_NOTOK,    0},
		{COND_MANUAL_CONTROLINPUT,    STATUS_NOTOK,    0},
		{COND_ACTIVE_CONTROLINPUT,    STATUS_NOTOK,    0}
	};

};
