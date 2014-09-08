#pragma once

#include "FreeRTOS.h"
#include "task.h"

	class Task {
		public:
			Task(const char* name, uint32_t stackSize, uint8_t priority):
				m_Name(name), m_StackSize(stackSize), m_Priority(priority){
				xTaskCreate(&runFunc, m_Name, m_StackSize, this, m_Priority, &m_TaskHandle);
			}

			virtual ~Task(void){
				vTaskDelete(m_TaskHandle);
			}

			inline const char* getTaskName(void) const {
				return m_Name;
			}
			inline uint32_t getStackSize(void) const {
				return m_StackSize;
			}
			inline uint8_t getPriority(void) const {
				return m_Priority;
			}
			inline TaskHandle_t getTaskHandle(void) const {
				return m_TaskHandle;
			}

			#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
				inline uint32_t getFreeStack(void) const { return uxTaskGetStackHighWaterMark(m_TaskHandle); }
			#endif
			#if (configGENERATE_RUN_TIME_STATS == 1)
				inline void getRunTimeStats(char* data) { vTaskGetRunTimeStats(data); }
			#endif

		protected:
			virtual void run(void) = 0;
			static void runFunc(void *p){
				(static_cast<Task*>(p))->run();
			}

		private:
			const char* m_Name;
			const uint32_t m_StackSize;
			const uint8_t m_Priority;
			TaskHandle_t m_TaskHandle;
	};

	// Helper function to start scheduler
	inline void schedulerStart(void) { vTaskStartScheduler(); }
