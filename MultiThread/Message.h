#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "CoreList.h"

//The message struct
//First element is enum for the receiver.  With it the manager dispatches the message to the proper core
//Second element is the message.  it is just int because each core may use its own set of enum messages.
//To include other data in a message, inheret this struct and add other data.
struct SMessage
{
	ECore Receiver;
	int Message;
	SMessage()
	{
		Receiver = (ECore)0;
		Message = 0;
	}
	SMessage(ECore receiver, int message)
	{
		Receiver = receiver;
		Message = message;
	}
	virtual ~SMessage(){}
};

#endif