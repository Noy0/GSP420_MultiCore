#ifndef SERVER_H
#define SERVER_H

#include "ServerClientBase.h"
#include "Client.h"

#include <map>
#include <queue>

typedef std::queue<pair<int, char*>> packetList;

#define MAX_PLAYERS 4

class Server : public SCBase
{
public:
	Server(int pID);
	~Server();

	HANDLE m_hListen;

	//Creates a new client for an incoming socket and adds it to the map.
	bool AcceptClient();
	//Disconnects the client who's ID is passed as a parameter and removes him from the map.
	void DisconnectClient(int pID);
	//Disconnects all clients and removes them all from the map.
	void DisconnectAllClients();
	//Returns a pointer to the client object from the map if the ID is known.
	Client* getClientWithID(int pID)const;
	//Returns the current number of clients in the client map.
	int getClientCount();
	//Sends a broadcast message to all clients in the LAN.
	void BroadcastMessage(int pPort, char* pMessage);
	//Binds the server to the port that was passed as a parameter.
	bool BindToPort(int pPort);
	//Listens for clients and accepts them if they try to establish a connection.
	static void ListenForClients(Server* pServer);
	//Sends whatever data is passed to all clients currently connected to the server.
	void SendToAllClients(char* pBuffer, int pBuffLen);
	//Sends data passed to a specific client. Returns number of bytes sent. 
	//If -1 is returned, the client is longer connected.
	int SendToClient(char* pBuffer, int pBuffLen, int pClientID);
	//Handles all incoming packets from all it's clients. WILL HAVE TO RETURN THE DATA.
	void HandlePackets(packetList* dl);
	//Handles all types of events received. Events are predefined by the networking engine to communicate
	//	special events between servers and clients.
	void HandleEvents(int pID, char* pPacket);
	//Returns f_Listen flag. Used to check signal the listening thread whether it should continue listening or stop.
	bool Listening();
	//Stops the listening thread immideately.
	void StopListening();

	typedef std::map<int, Client*> ClientSocketMap;
private:
	ClientSocketMap m_ClientList;

	int m_NextClientID;
	//While this flag is true, the server will be able to listen for clients. When this flag is set to false
	//	the listen thread will end and it will have to be restarted when the server wants to listen again.
	bool f_Listen;
};

#endif //SERVER_H