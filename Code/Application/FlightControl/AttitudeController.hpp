#pragma once

#include "Framework.hpp"
#include "PID.hpp"

class AttitudeController{
	public:
		AttitudeController(){
			reset();
		}

		void update(APP_Attitude_I& attitude, APP_Control_I& control){


			if(i_term_reset){
					if(control.throttle > 400){
						pid_pitch_angle.set_ki(angle_ki);
						pid_roll_angle.set_ki(angle_ki);
						pid_pitch_rate.set_ki(velocity_ki);
						pid_roll_rate.set_ki(velocity_ki);
						pid_yaw_rate.set_ki(yaw_ki);
					}
				}

				//Run Attitude PID algorithms:
				//Compute the angular velocity needed to correct the angle error:
				setpoint_pitch_velocity  = pid_pitch_angle.update(attitude.pitch, control.pitch);
				setpoint_roll_velocity 	 = pid_roll_angle.update(attitude.roll, control.roll);


				//Compute the motor difference needed to achieve this angular velocity:
				pitch_cmd = pid_pitch_rate.update(-attitude.pitch_velocity, setpoint_pitch_velocity);
				roll_cmd  = pid_roll_rate.update(-attitude.roll_velocity, setpoint_roll_velocity);
				yaw_cmd   = pid_yaw_rate.update(-attitude.yaw_velocity, control.yaw*6.0);

		}

		void get_commands(float& pitch, float& roll, float& yaw){
			pitch = pitch_cmd;
			roll = roll_cmd;
			yaw = yaw_cmd;
		}

		void get_velocity_commands(float& pitch, float& roll, float& yaw){
			pitch = setpoint_pitch_velocity;
			roll = setpoint_roll_velocity;
			yaw = yaw_cmd;
		}

		float get_pitch_cmd(){return pitch_cmd;}
		float get_roll_cmd(){return roll_cmd;}
		float get_yaw_cmd(){return yaw_cmd;}

		void reset(){
			pid_pitch_angle.reset_integrator();
			pid_roll_angle.reset_integrator();
			pid_pitch_rate.reset_integrator();
			pid_roll_rate.reset_integrator();
			pid_yaw_rate.reset_integrator();

			pid_pitch_angle.set_ki(0);
			pid_roll_angle.set_ki(0);
			pid_pitch_rate.set_ki(0);
			pid_roll_rate.set_ki(0);
			pid_yaw_rate.set_ki(0);

			i_term_reset = true;
		}

		void reset_constants(){
			float pid_p = Globals::angle_control_p.get();
			float pid_i = Globals::angle_control_i.get();
			float pid_d = Globals::angle_control_d.get();

			pid_pitch_angle.set_constants(pid_p, 0.0, pid_d, 1000.0);
			pid_roll_angle.set_constants(pid_p, 0.0, pid_d, 1000.0);

			//This will be setup at a later time (when we are in the air!)
			angle_ki = pid_i;

			pid_p = Globals::rate_control_p.get();
			pid_i = Globals::rate_control_i.get();
			pid_d = Globals::rate_control_d.get();

			pid_pitch_rate.set_constants(pid_p, 0.0, pid_d, 1000.0);
			pid_roll_rate.set_constants(pid_p, 0.0, pid_d, 1000.0);
			velocity_ki = pid_i;

			pid_p = Globals::yaw_control_p.get();
			pid_i = Globals::yaw_control_i.get();
			pid_d = Globals::yaw_control_d.get();

			pid_yaw_rate.set_constants(pid_p, 0.0, pid_d, 1000.0);
			yaw_ki = pid_i;
		}

	private:
		PID pid_pitch_angle;
		PID pid_roll_angle;

		PID pid_pitch_rate;
		PID pid_roll_rate;
		PID pid_yaw_rate;

		float pitch_cmd = 0;
		float roll_cmd  = 0;
		float yaw_cmd   = 0;

		float setpoint_roll_velocity;
		float setpoint_pitch_velocity;

		bool i_term_reset = true;
		float angle_ki = 0;
		float velocity_ki = 0;
		float yaw_ki = 0;

};

