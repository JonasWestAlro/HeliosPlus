#pragma once
/**************************************************************************//**
 * @file     MessageDistribution.hpp
 * @brief
 * @version  V1.0
 * @date     13. October 2014
 *
 * @note
 * This file is a part of the framework develop for Helios Flightcontroller
 *
 ******************************************************************************/


//!< Forward declaration
class Message;

#include "Mutex.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "MessageQueue.hpp"

//!todo Make a Framework Configuration file just like in freeRTOS and put this define there
#define FRAMEWORK_MAX_MESSAGE_SUBSCRIBERS	50

class MessageDistribution {
	public:
		static MessageDistribution& getInstance(void){
			static MessageDistribution mds;
			return mds;
		}

		//!TODO-JWA: We should consider to use an optimized way of organizing subscribers and
		//			 finding them again when broadcasting.
		//			 We could sort the subscriber array and use binary search, or even hashing!
		bool subscribe(MessageType msg_type, MessageQueue& queue);
		void broadcast(Message& msg);

	private:
		MessageDistribution(void) {}

		struct subscriber{
			MessageType msg_type;
			MessageQueue* queue;
		};

		struct subscriber subscriberList[FRAMEWORK_MAX_MESSAGE_SUBSCRIBERS];
		uint8_t subscriberCount;

		Mutex mutex;
};

