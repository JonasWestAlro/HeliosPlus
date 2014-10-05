#pragma once

#include "Framework.hpp"
#include "PID.hpp"
#include "arm_math.h"

#define CONSTRAIN(x,min,max) \
	if(x > max) x = max;\
	if(x < min) x = min;


class ThrottleController{
public:
	ThrottleController(){}

	float update(APP_Navigation_I& navigation, APP_Attitude_I attitude, APP_Control_I& control){
		static ControlMode last_control_mode = CONTROLMODE_MANUAL_THROTTLE;

		//Decide whether we are going to use manual or altitude hold throttle:
		if(control.control_mode == CONTROLMODE_MANUAL_THROTTLE){
			throttle = control.throttle;
		}else{
			//We are in altitude hold mode
			if(last_control_mode == CONTROLMODE_MANUAL_THROTTLE){
				//TODO-JWA: We should reset the PID controllers here!
			}

			throttle = process_altitude_hold(navigation, control.altitude);
		}

		if(tilt_correction_enabled){
			process_tilt_correction(attitude);
		}

		last_control_mode = control.control_mode;
		return throttle;
	}

	float process_altitude_hold(APP_Navigation_I& navigation, float setpoint){
		altitude_setpoint = setpoint;

		if(navigation.position.z < 10 && altitude_setpoint < 10){
			//We are landed!
			throttle = throttle_nominel - 150;
			altitude_pid.reset_integrator();
			velocity_pid.reset_integrator();

		}else{
			float error = altitude_setpoint - navigation.position.z;

			if(error > leash){
				real_target = navigation.position.z + leash;
			}else if(error < -leash){
				real_target =  navigation.position.z - leash;
			}else{
				real_target = altitude_setpoint;
			}

			float throttle_correction  = altitude_pid.update(navigation.position.z, real_target);
			float z_damping_correction = -velocity_pid.update(0, navigation.velocity.z);

			throttle = throttle_nominel + throttle_correction + z_damping_correction;

			if(throttle < 200) throttle = 200;
		}
		return throttle;
	}

	void process_tilt_correction(APP_Attitude_I& attitude){
		float pitch_rad = attitude.pitch*(2.0*M_PI/360.0);
		float roll_rad  = attitude.roll*(2.0*M_PI/360.0);

		float tilt_correction = throttle / (arm_cos_f32(pitch_rad*0.8) * arm_cos_f32(roll_rad*0.8));
		tilt_correction = contrain((tilt_correction - throttle), 0, 50);

		throttle += tilt_correction;
	}

	void get_throttle_cmd(float& result){
		result = throttle;
	}

	void set_nom_throttle(float _throttle_nom){
		throttle_nominel = _throttle_nom;
	}

	void reset_constants(){
		float imax = Globals::throttle_control_alt_imax.get();

		float pid_p = Globals::throttle_control_vel_p.get();
		float pid_i = Globals::throttle_control_vel_i.get();
		float pid_d = Globals::throttle_control_vel_d.get();
		velocity_pid.set_constants(pid_p, pid_i, pid_d, imax);

		pid_p = Globals::throttle_control_alt_p.get();
		pid_i = Globals::throttle_control_alt_i.get();
		pid_d = Globals::throttle_control_alt_d.get();
		altitude_pid.set_constants(pid_p, pid_i, pid_d, imax);

		leash = Globals::throttle_control_alt_leash.get();

		altitude_pid.reset_integrator();
		velocity_pid.reset_integrator();

	}

	void set_altitude_setpoint(float setpoint){
		altitude_setpoint = setpoint;
	}

private:
	 PID altitude_pid;
	 PID velocity_pid;

	 bool tilt_correction_enabled = false;

	 float altitude_setpoint 	= 0;
	 float throttle_nominel  	= 450.0;
	 float throttle 	  		= 0;
	 float leash 		  		= 0;
	 float real_target 	  		= 0;

	 float contrain(float variable, float min, float max){
	 	if(variable > max) return max;
	 	if(variable < min) return min;

	 	return variable;
	 }

};
