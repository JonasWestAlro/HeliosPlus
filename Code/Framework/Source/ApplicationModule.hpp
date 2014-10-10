#pragma once

#include "EEPROM.hpp"
#include "Messenger.hpp"
#include "Task.hpp"
#include "Timing.hpp"

class ApplicationModule : public Task {
	public:
		ApplicationModule(const char* name, uint32_t stackSize, uint8_t priority, uint32_t eeprom_size = 0)
		: Task(name, stackSize, priority),
		  eeprom(name, eeprom_size),
		  messenger()
		{

		}

		void run(void){
			Message msg;
			while(1){
				last_wake_tick = get_tick();

				while(messenger.try_receive(&msg)){
					handle_message(msg);
				}
				measured_frequency = 1.0/Time.get_time_since_sec(measurement_timestamp);

				measurement_timestamp = Time.get_timestamp();
				task();
				measured_duration = Time.get_time_since_us(measurement_timestamp);

				measurement_timestamp = Time.get_timestamp();
				delay_until(&last_wake_tick, ticks_to_delay);
			}
		}

		void set_frequency(uint16_t frequency_){
			frequency = frequency_;

			float time_between_executions = 1.0f/(float)frequency;
			float time_between_ticks = 1.0f/(float)get_tick_frequency();

			float result = time_between_executions/time_between_ticks;

			//Check that the result makes sense:
			if(result > 2){
				ticks_to_delay = (portTickType)result;
			}else{
				//ERROR!
				while(1){}
			}
		}

		float get_measured_frequency(){
			return measured_frequency;
		}

		uint32_t get_measured_duration(){
			return measured_duration;
		}

		virtual void task() = 0;
		virtual void handle_message(Message& msg) = 0;

		Messenger* get_messenger(){
			return &messenger;
		}

	protected:
		EEPROM eeprom;
		Messenger messenger;

		uint16_t frequency = 10;

		portTickType ticks_to_delay = 2000;
		portTickType last_wake_tick = 0;

		float measured_frequency = 0;
		uint32_t measurement_timestamp = 0;
		uint32_t measured_duration;

	private:


};

