#pragma once

#include "Framework.hpp"

class MessageDistribution {
	public:
		static MessageDistribution& getInstance(void){
			static MessageDistribution mds;
			return mds;
		}

		//TODO-JWA: We should consider to use an optimized way of organizing subscribers and
		//			finding them again when broadcasting.
		//			We could sort the subscriber array and use binary search, or even hashing!
		bool subscribe(MessageType msg_type, MessageQueue& queue){
			bool result = false;

			mutex.take();
			if (subscriberCount < FRAMEWORK_MAX_MESSAGE_SUBSCRIBERS) {
				subscriberList[subscriberCount].msg_type = msg_type;
				subscriberList[subscriberCount].queue = &queue;
				subscriberCount++;
				result = true;
			}
			mutex.release();

			return result;
		}

		void broadcast(Message& msg){
			uint8_t i;
			mutex.take();
			for(i = 0; i < subscriberCount; i++) {
				if (subscriberList[i].msg_type == msg.message_type) {
					subscriberList[i].queue->send(&msg);
				}
			}
			mutex.release();
		}

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

