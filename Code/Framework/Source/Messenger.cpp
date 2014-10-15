#include "Messenger.hpp"

Messenger::Messenger(): msg_queue(10, sizeof(Message))
{}

void Messenger::subscribe(MessageType msg_type){
	MessageDistribution::getInstance().subscribe(msg_type, msg_queue);
}

void Messenger::send_to(Messenger* receiver, Message msg){
	msg.sender   = this;
	msg.receiver = receiver;
	receiver->msg_queue.send(&msg);
}

void Messenger::send_to(Messenger* receiver, Message* msg){
	msg->sender   = this;
	msg->receiver = receiver;
	receiver->msg_queue.send(msg);
}

void Messenger::respond(Message& msg, Message& response){
	response.sender   = this;
	response.receiver = msg.sender;
	response.receiver->msg_queue.send(&msg);
}

void Messenger::respond(Message& msg, Message response){
	response.sender   = this;
	response.receiver = msg.sender;
	response.receiver->msg_queue.send(&msg);
}

bool Messenger::try_receive(void* ptr){
	return msg_queue.try_receive(ptr);
}

void Messenger::broadcast(Message* msg){
	msg->sender   = this;
	msg->receiver = 0;
	MessageDistribution::getInstance().broadcast(*msg);
}

void Messenger::broadcast(Message msg){
	msg.sender 	 = this;
	msg.receiver = 0;
	MessageDistribution::getInstance().broadcast(msg);
}

void Messenger::broadcast(MessageType type){
	Message msg(type);
	msg.sender 		= this;
	msg.receiver 	= 0;
	MessageDistribution::getInstance().broadcast(msg);
}

void Messenger::broadcast(MessageType type, uint8_t enum_value){
	Message msg(type);
	msg.sender 		= this;
	msg.receiver 	= 0;
	msg.set_enum(enum_value);
	MessageDistribution::getInstance().broadcast(msg);
}
