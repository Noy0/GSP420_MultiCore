#include "Client.h"

Client::Client(int pID)
{
	setID(pID);

	m_Timer = new Timer();
	//nextMsgID = 0;
	//f_NewMessage = false;
	//f_Go = true;
	//DWORD bListenID;
	//m_hBroadcastListen = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) &broadcastListener, this, NULL, &bListenID);
}

//void Client::broadcastListener(Client* pClient)
//{
//	//Trying to implement a system where the client can listen for broadcast messages on a seperate thread.
//	//things that might go wrong:
//	//	no port has been assigned (but it is a client so there may be no need to bind)
//	//	messages must be simple to understand and be an int for a flag or something.
//	//	there's no implemented system that will act according to the message.
//	char* tempBuffer = NULL;
//	while(pClient->f_Go)
//	{
//		if(pClient->m_Socket->RecvBroadcast(tempBuffer, sizeof(tempBuffer)) > 0)
//		{
//			output("New broadcast recieved.");
//			pClient->newMessage(tempBuffer);
//			pClient->f_NewMessage = true;
//		}
//		else
//		{
//			//idk :( just chillax i/g
//		}
//	}
//}
//
//void Client::newMessage(char* pMsg)
//{
//	m_BroadcastList.insert(std::make_pair(++nextMsgID, pMsg));
//}
//
//void Client::deleteMessage(int pMsgID)
//{
//	m_BroadcastList.erase(m_BroadcastList.find(pMsgID));
//}

void Client::Connect(char *pServerAddress, int pPortNumber)
{
	if(m_Socket->Connect(pServerAddress, pPortNumber))
	{
		output("Client has connected to the server.");
		m_Timer->Start(10);
		output("Timer has been started.");
	}
	else
	{
		output("Client was unable to connect to the server.");
	}
}

void Client::Disconnect()
{
	m_Socket->Disconnect();
	m_Timer->Reset();
	output("Client have diconnected from the server.");
}

int Client::ReceiveData(char *pBuffer)
{
	//Since this method is constantly being called, it will check to make sure that there that the timer hasn't passed
	//	10 seconds. If it has, then it will create an event packet and sent it to the server to signal it that the
	// client is still connected.
	if((m_Timer->Check() == TIME_OVER) && (getID() == 0))
	{
		output("Timer is over 10 seconds, echo event sent to server.");
		EventPacket ep;
		ep.m_Event = SE_ECHO;
		ep.m_Header.m_Type = 1;
		ep.m_Header.m_Length = sizeof(int);

		char eventBuffer[128];
		ZeroMemory(&eventBuffer, sizeof(eventBuffer));
		memcpy(&eventBuffer, &ep, sizeof(EventPacket));
		SendData(eventBuffer, sizeof(EventPacket));
	}
	return EC(m_Socket->GetPacket(pBuffer));
}

int Client::SendData(char* pBuffer, int pBufferLen)
{
	if(getID() == 0)
	{
		m_Timer->Start(10);
		output("Data sent, timer restarted.");
	}
	return EC(m_Socket->Send(pBuffer, pBufferLen));
}

Client::~Client()
{
	//CloseHandle(m_hBroadcastListen);
	//delete broadcast message list.
}