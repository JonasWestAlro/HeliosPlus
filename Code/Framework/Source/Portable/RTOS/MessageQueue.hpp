#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

class MessageQueue
{
	public:
		MessageQueue(uint16_t messageQueueSize, uint16_t itemSize){
				m_QueueHandle = xQueueCreate(messageQueueSize, itemSize);
				if (m_QueueHandle == NULL) {
					while(1);
				}
			}

		~MessageQueue(void){
			vQueueDelete(m_QueueHandle);
		}

		bool send(void *ptr){
			return xQueueSend(m_QueueHandle, ptr, 0);
		}

		bool sendFromIsr(void *ptr){
			return xQueueSendFromISR(m_QueueHandle, ptr, NULL);
		}
		bool sendPend(void *ptr){
			return xQueueSend(m_QueueHandle, ptr, portMAX_DELAY);
		}

		bool try_receive(void* ptr){
			return xQueueReceive(m_QueueHandle, ptr, 1);
		}

		bool receiveFromISR(void *ptr){
			return xQueueReceiveFromISR(m_QueueHandle, ptr, 0);
		}

		bool receivePend(void *ptr){
			return xQueueReceive(m_QueueHandle, ptr, portMAX_DELAY);
		}

		uint16_t getFreeSpace(void){
			return uxQueueSpacesAvailable(m_QueueHandle);
		}

	private:
		xQueueHandle m_QueueHandle;
};
