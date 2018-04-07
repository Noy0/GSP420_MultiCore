#include "Server.h"

Server::Server(int pID)
{
	this->setID(pID);
	f_Listen = false;
	m_NextClientID = 1;
}

bool Server::AcceptClient()
{
	Client* client = new Client(m_NextClientID);
	m_NextClientID++;

	if(m_Socket->Accept(client->m_Socket))
	{
		m_ClientList.insert(std::make_pair(client->getID(), client));
		client->m_Socket->StartRecvThread();
		output("Server has accepted a client's socket.");
		return true;
	}
	else
	{
		output("Server could not accept the client's socket.");
		return false;
	}
}

void Server::DisconnectClient(int pID)
{
	getClientWithID(pID)->Disconnect();

	// Deletes entities without memory leaks ====================================================================
	//Create an iterator so that we can look for the entity using the ID.
	ClientSocketMap::const_iterator entity = m_ClientList.find(pID);
	//Then create a pointer that will point to that location so that we have a way of deleting the memory.
	Client* data_pointer_is_pointing_to = (*entity).second;
	//Finally delete the memory being pointed to.
	delete data_pointer_is_pointing_to;
	//And erase the entity from the map. Goodbye :(
	m_ClientList.erase(m_ClientList.find(pID));
	//===========================================================================================================
}

void Server::DisconnectAllClients()
{
	for(ClientSocketMap::const_iterator entity = m_ClientList.begin(); entity != m_ClientList.end(); ++entity)
	{
		Client* data_pointer_is_pointing_to = (*entity).second;
		data_pointer_is_pointing_to->Disconnect();
		delete data_pointer_is_pointing_to;
	}
	m_ClientList.clear();
	m_NextClientID = 1;
}

Client* Server::getClientWithID(int pID)const
{
	ClientSocketMap::const_iterator client = m_ClientList.find(pID);

	return client->second;
}

int Server::getClientCount()
{
	return (int) m_ClientList.size();
}

void Server::BroadcastMessage(int pPort, char* pMessage)
{
	EC(m_Socket->BindForBroadcast(pPort));
	m_Socket->SendBroadcast(pMessage, sizeof(pMessage));
	m_Socket->Disconnect();
}

bool Server::BindToPort(int pPort)
{
	if(EC(m_Socket->Bind(pPort)) == SOCKET_ERROR)
	{
		output("Server could not bind to a port.");
		return false;
	}
	else
	{
		DWORD ListenID;
		output("Server has binded to a port and the listen thread will begin.");
		f_Listen = true;
		m_hListen = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) &ListenForClients, this, NULL, &ListenID);
		return true;
	}
}

void Server::SendToAllClients(char *pBuffer, int pBuffLen)
{
	for(ClientSocketMap::const_iterator i = m_ClientList.begin(); i != m_ClientList.end(); i++)
	{
		EC(i->second->SendData(pBuffer, pBuffLen));
	}
	output("Server has sent a message out to all clients.");
}

int Server::SendToClient(char* pBuffer, int pBuffLen, int pClientID)
{
	//use error checking;
	output("Server has sent a message to a client.");
	return EC(getClientWithID(pClientID)->SendData(pBuffer, pBuffLen));
}

void Server::HandlePackets(packetList* dl)
{
	//call the GetPacket method from all the client's m_Socket (just loop through all of them, and w/e the buffer
	// returns, then pass it to a function that will handle it if it's an event.
	//use the goto thing in the cplusplus website.
restart:
	for(ClientSocketMap::iterator i = m_ClientList.begin(); i != m_ClientList.end(); i++)
	{
		char data[256];
		ZeroMemory(&data, sizeof(data));
		switch(EC(i->second->ReceiveData(data)))
		{
		case -1:
			//CONNECTION IS CLOSED, SO DISCONNECT UR SIDE AND REMOVE FROM LIST.
			{
				DisconnectClient(i->second->getID());
				goto restart;
				//i = m_ClientList.begin();
			}
			break;
		case 0:
			{
				//ummm...no data recieve yet? ...idk, start the echo timer?
				switch(i->second->m_Timer->Check())
				{
				case TIME_OFF:
					output("Starting timer for client.");
					i->second->m_Timer->Start(10);
					break;
				case TIME_UNDER:
					//everything is good.
					break;
				case TIME_OVER:
					//output("Timer for client is now over 10 seconds, waiting 10 more seconds.");
					//YOU COULD EITHER TRY TO CONTACT THE CLIENT TO SEE IF HE'S THERE OR JUST CHILL.
					break;
				case TIME_UP:
					//DELETE THAT MOFO OFF THE LIST CUZ HE PROLLY LOST CONNECTION.
					{
						output("Timer for client has now reached 20 seconds and will be disconnected.");
						DisconnectClient(i->second->getID());
						//i = m_ClientList.begin();
						goto restart;
					}
					break;
				default:
					//Timer is under 10 seconds so don't worry, just relax.
					break;
				}
			}
			break;
		default:
			{
				PacketHeader header;
				ZeroMemory(&header, sizeof(PacketHeader));
				memcpy((void*)&header, (void*)&data, sizeof(PacketHeader));
				
				if(header.m_Type == 1)
				{
					//The header defined the received data as an event, call the HandleEvents() method.
					HandleEvents(i->second->getID(), data);
				}
				else
				{
					char* temp = new char[256];
					memcpy(temp, data, sizeof(data));
					//Store the data received in the data queue.
					dl->push(make_pair(i->second->getID(), temp));
				}
			}
		}
	}
	//delete data somehow since it is a new char*
}

void Server::HandleEvents(int pID, char* pPacket)
{
	//Create an event packet variable so that we can memcpy the data pPacket into.
	EventPacket ep;
	ZeroMemory(&ep, sizeof(EventPacket));
	//Copy the data into the packet so that we can access the information.
	memcpy((void*)&ep, (void*)pPacket, sizeof(EventPacket));

	switch(ep.m_Event)
	{
	case SE_ECHO:
		//DELETE pPacket since is is a random new char*
		output("Echo received from client, restarting timer.");
		getClientWithID(pID)->m_Timer->Start(10);
		break;
	default:
		//no defined event.
		break;
	}
}

bool Server::Listening()
{
	return f_Listen;
}

void Server::StopListening()
{
	f_Listen = false;
	//TerminateThread(m_hListen, 0);
	
}

void Server::ListenForClients(Server* pServer)
{
	output("Server is now listening.");
	while(pServer->Listening())
	{
		pServer->m_Socket->Listen();

		//if(pServer->Listening())
		//{
			pServer->AcceptClient();
		//}
	}
	output("Server has now stopped listening.");
}

Server::~Server()
{
	DisconnectAllClients();
	f_Listen = false;
	TerminateThread(m_hListen, 0);
}