
#ifndef NETWORKING_H
#define NETWORKING_H

#include "Server.h"
#include "Client.h"



class NetworkingEngine
{
public:
	NetworkingEngine();
	~NetworkingEngine();

	//Switches the application from a client to a server.
	void SwitchToServer();
	//Switches the application from a server to a client.
	void SwitchToClient();

	//Sends data through the active mode (server or client).
	int SendData(char* pBuffer, int pBufferLen);
	//Recieves data through the active mode (server or client).
	void RecvData();

	packetList m_Packets;

public:
	//Server mode object.
	Server* m_Server;
	//Client mode object.
	Client* m_Client;

	//Flags =================================================================================================
	//Flag to determine what mode the engine is in, true means it is on server mode, false means client mode.
	bool f_ServerMode;
	//Flag that determines whether the server has binded or not.
	bool f_ServerIsBinded;
	//Flag that determines whether the client has established a connection or not.
	bool f_ClientConnected;
	//Flag that lets the client or server know when he's recieved data.
	bool f_DataReceived;

public:
	//Server methods ==================================================================================
	//When in server mode, binds the server to a port so that it can listen and send/recieve data.
	bool s_Bind(int pPort);
	//When in server mode, shuts down the socket and closes it.
	bool s_UnBind();

	//Client methods ==================================================================================
	//When in client mode, connects to a server.
	bool c_Connect(char* pAddress, int pPort);
	//When in client mode, disconnects from the active connection.
	bool c_Disconnect();
};

#endif //NETWORKING_H