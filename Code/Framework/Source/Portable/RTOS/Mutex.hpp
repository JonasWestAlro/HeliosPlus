/**************************************************************************//**
 * @file     Mutex.hpp
 * @brief    Wrapper for freeRTOS mutex
 * @version  V1.0
 * @date     13. October 2014
 *
 * @note
 * This file is a part of the framework develop for Helios Flightcontroller
 *
 ******************************************************************************/

#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

class Mutex{
	public:
		//! Constructor
		Mutex(void){
			m_MutexHandle = xSemaphoreCreateMutex();
			if (m_MutexHandle == NULL) {
				while(1);
			}
		}

		//! Copy Constructor
		Mutex(const Mutex& mutex){
			m_MutexHandle = mutex.m_MutexHandle;
		}

		//! Destructor
		~Mutex(void){
			//vSemaphoreDelete(m_MutexHandle);
		}

		//! Copy assignment
		Mutex& operator=( Mutex& other)
		{
			if(this != &other){
				m_MutexHandle = other.get_handle();
			}
		    return *this;
		}

		//! Take a mutex and block until it is taken
		void take(void){
			xSemaphoreTake(m_MutexHandle, portMAX_DELAY);
		}


		/** @brief  	Try take mutex. Return immediately
		  * @retval 	True		If mutex was taken
		  * @retval		False		If mutex was NOT taken
		  */
		bool try_take(void){
			return xSemaphoreTake(m_MutexHandle, 0);
		}

		/** @brief  	Try take mutex. Return immediately
		  * @retval 	True		If mutex was released
		  * @retval		False		If mutex was NOT released. Will happen if the mutex was not already taken.
		  */
		bool release(void){
			return xSemaphoreGive(m_MutexHandle);
		}

		/** @brief  	Get the Mutex Handle
		  * @return		See #SemaphoreHandle_t
		  */
		SemaphoreHandle_t get_handle(){
			return m_MutexHandle;
		}

	private:
		SemaphoreHandle_t  m_MutexHandle;	//!< Mutex Handle
};

