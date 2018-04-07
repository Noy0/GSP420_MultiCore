#ifndef CLIENT_H
#define CLIENT_H

#include "ServerClientBase.h"
#include <map>

class Client : public SCBase
{
public:
	Client(int pID);
	~Client();

	//Connects to the specified address on a certain port.
	void Connect(char* pServerAddress, int pPortNumber);
	//Disconnects the client from the current connection.
	void Disconnect();
	
	//Receives data from the connected socket and stores in the passed character pointer.
	int ReceiveData(char* pBuffer);
	int SendData(char* pBuffer, int pBufferLen);

	Timer* m_Timer;

//private:
//	typedef std::map<int, char*> tm_Broadcast;
//	tm_Broadcast m_BroadcastList;
//
//public:
//	//A flag that announces a new message, true means a new message, false means no new messages.
//	bool f_NewMessage;
//	//A flag that signals the broadcast listening thread to exit.
//	bool f_Go;
//	//Used to keep track of available ID's for incoming broadcast messages.
//	int nextMsgID;
//	//Handle to the broadcast listening thread.
//	HANDLE m_hBroadcastListen;
//	//A thread that constantly listens for broadcasts.
//	static void broadcastListener(Client* pClient);
//	//Adds a new message to the broadcast list.
//	void newMessage(char* pMsg);
//	//Deletes a message from the broadcast list using it's ID.
//	void deleteMessage(int pMsgID);

public:
	//This method is used by the server to check incoming packets.
	
};

#endif //CLIENT_H