#ifndef NETCORE_MESSAGE_H
#define NETCORE_MESSAGE_H

#include "..\\Message.h"


enum ENetCoreMessage
{
	MSG_SWITCHTOCLIENT = 1,
	MSG_SWITCHTOSERVER,
	MSG_SERVER_BIND,
	MSG_SERVER_UNBIND,
	MSG_CLIENT_CONNECT,
	MSG_CLIENT_DISCONNECT,
	MSG_SENDDATA,
	MSG_GETCLIENTCOUNT
};

struct SMessageSwitchToClient : public SMessage
{
	SMessageSwitchToClient(): SMessage(CORE_NETWORKING, MSG_SWITCHTOCLIENT){}
};

struct SMessageSwitchToServer : public SMessage
{
	SMessageSwitchToServer(): SMessage(CORE_NETWORKING, MSG_SWITCHTOSERVER){}
};

struct SMessageServerBind : public SMessage
{
	int m_Port;

	SMessageServerBind(int pPort) : SMessage(CORE_NETWORKING, MSG_SERVER_BIND)
	{
		m_Port = pPort;
	}
};

struct SMessageServerUnbind : public SMessage
{
	SMessageServerUnbind() : SMessage(CORE_NETWORKING, MSG_SERVER_UNBIND){}
};

struct SMessageClientConnect : public SMessage
{
	SMessageClientConnect(char* pAddress, int pPort) : SMessage(CORE_NETWORKING, MSG_CLIENT_CONNECT)
	{
		m_Address = pAddress;
		m_Port = pPort;
	}
	
	char* m_Address;
	int m_Port;
};

struct SMessageClientDisconnect : public SMessage
{
	SMessageClientDisconnect() : SMessage(CORE_NETWORKING, MSG_CLIENT_DISCONNECT){}
};

//struct SMessageSendData : public SMessage
//{
//	char* m_Buffer;
//
//	SMessageSendData(char* pBuffer) : SMessage(CORE_NETWORKING, MSG_SENDDATA)
//	{
//		m_Buffer = pBuffer;
//	}
//};

struct SMessageSendData : public SMessage
{
	char* m_Buffer;
	int m_Size;

	SMessageSendData(char* pBuffer, int pSizeOfPacket) : SMessage(CORE_NETWORKING, MSG_SENDDATA)
	{
		m_Buffer = pBuffer;
		m_Size = pSizeOfPacket;
	}
};

struct SMessageGetClientCount : public SMessage
{
	SMessageGetClientCount(int* pCount) : SMessage(CORE_NETWORKING, MSG_GETCLIENTCOUNT)
	{
		Count = pCount;
	}
	int* Count;
};

#endif //NETCORE_MESSAGE_H