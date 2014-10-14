#include "MessageDistribution.hpp"


//TODO-JWA: We should consider to use an optimized way of organizing subscribers and
//			finding them again when broadcasting.
//			We could sort the subscriber array and use binary search, or even hashing!
bool MessageDistribution::subscribe(MessageType msg_type, MessageQueue& queue){
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

void MessageDistribution::broadcast(Message& msg){
	uint8_t i;
	mutex.take();
	for(i = 0; i < subscriberCount; i++) {
		if (subscriberList[i].msg_type == msg.type) {
			subscriberList[i].queue->send(&msg);
		}
	}
	mutex.release();
}

