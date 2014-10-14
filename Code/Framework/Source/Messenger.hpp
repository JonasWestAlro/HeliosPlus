#pragma once
/**************************************************************************//**
 * @file     Messenger.hpp
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
class MessageDistribution;


#include "Message.hpp"
#include "MessageQueue.hpp"
#include "MessageTypes.hpp"
#include "MessageDistribution.hpp"
#include <stdint.h>


class Messenger{
	public:
		//! Default constructor
		Messenger();

		void subscribe(MessageType msg_type);

		void send_to(Messenger* receiver, Message msg);
		void send_to(Messenger* receiver, Message* msg);

		void respond(Message& msg, Message& response);
		void respond(Message& msg, Message response);

		bool try_receive(void* ptr);

		void broadcast(Message* msg);
		void broadcast(Message msg);
		void broadcast(MessageType type);
		void broadcast(MessageType type, uint8_t enum_value);

	private:
		MessageQueue msg_queue;
};
