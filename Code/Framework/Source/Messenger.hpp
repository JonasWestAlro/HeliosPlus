#pragma once

#include "Framework.hpp"

class Messenger{
	public:
		Messenger():
			msg_queue(10, sizeof(Message))
		{

		}

		void subscribe(MessageType msg_type){
			MessageDistribution::getInstance().subscribe(msg_type, msg_queue);
		}

		void send_to(Messenger* receiver, Message msg){
			msg.sender   = this;
			msg.receiver = receiver;
			receiver->msg_queue.send(&msg);
		}

		void send_to(Messenger* receiver, Message* msg){
			msg->sender   = this;
			msg->receiver = receiver;
			receiver->msg_queue.send(msg);
		}

		void respond(Message& msg, Message& response){
			response.sender   = this;
			response.receiver = msg.sender;
			response.receiver->msg_queue.send(&msg);
		}

		void respond(Message& msg, Message response){
			response.sender   = this;
			response.receiver = msg.sender;
			response.receiver->msg_queue.send(&msg);
		}

		bool try_receive(void* ptr){
			return msg_queue.try_receive(ptr);
		}

		void broadcast(Message* msg){
			msg->sender 	 = this;
			msg->receiver = 0;
			MessageDistribution::getInstance().broadcast(*msg);
		}

		void broadcast(Message msg){
			msg.sender 	 = this;
			msg.receiver = 0;
			MessageDistribution::getInstance().broadcast(msg);
		}

		void broadcast(MessageType type){
			Message msg(type);
			msg.sender 		= this;
			msg.receiver 	= 0;
			MessageDistribution::getInstance().broadcast(msg);
		}

	private:
		MessageQueue msg_queue;
};
