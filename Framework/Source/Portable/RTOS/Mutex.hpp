#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

class Mutex{
	public:
		Mutex(void){
			m_MutexHandle = xSemaphoreCreateMutex();
			if (m_MutexHandle == NULL) {
				while(1);
			}
		}

		Mutex(const Mutex& mutex){
			m_MutexHandle = mutex.m_MutexHandle;
		}

		~Mutex(void){
			//vSemaphoreDelete(m_MutexHandle);
		}

		// Block until mutex is taken
		void take(void){
			xSemaphoreTake(m_MutexHandle, portMAX_DELAY);
		}

		// Return true if Mutex was taken
		// Return false if not
		bool try_take(void){
			return xSemaphoreTake(m_MutexHandle, 0);
		}


		/* @return pdTRUE if the semaphore was released.  pdFALSE if an error occurred.
		 * Semaphores are implemented using queues.  An error can occur if there is
		 * no space on the queue to post a message - indicating that the
		 * semaphore was not first obtained correctly.
		 */
		bool release(void){
			return xSemaphoreGive(m_MutexHandle);
		}

		Mutex& operator=( Mutex& other) // copy assignment
		{
		    m_MutexHandle = other.get_handle();
		    return *this;
		}

		SemaphoreHandle_t get_handle(){
			return m_MutexHandle;
		}

	private:
		SemaphoreHandle_t  m_MutexHandle;
};

