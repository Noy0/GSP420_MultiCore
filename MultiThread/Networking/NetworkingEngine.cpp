#include "NetworkingEngine.h"

NetworkingEngine::NetworkingEngine()
{
	m_Client = NULL;
	m_Server = NULL;

	//start as a client so that switches kill the threads.
	SwitchToClient();
}

bool NetworkingEngine::c_Connect(char* pAddress, int pPort)
{
	if(f_ServerMode)
	{
		return false;
	}
	else
	{
		m_Client->Connect(pAddress, pPort);
		f_ClientConnected = true;
		return true;
	}
}

bool NetworkingEngine::c_Disconnect()
{
	if(f_ServerMode)
	{
		return false;
	}
	else
	{
		m_Client->Disconnect();
		f_ClientConnected = false;
		return true;
	}
}

bool NetworkingEngine::s_Bind(int pPort)
{
	if(f_ServerMode)
	{
		f_ServerIsBinded = true;
		return m_Server->BindToPort(pPort);
	}
	else
	{
		return false;
	}
}

bool NetworkingEngine::s_UnBind()
{
	if(f_ServerMode)
	{
		if(f_ServerIsBinded)
		{
			m_Server->m_Socket->Disconnect();
			f_ServerIsBinded = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int NetworkingEngine::SendData(char* pBuffer, int pBufferLen)
{
	if(f_ServerMode)
	{
		if(f_ServerIsBinded)
		{
			m_Server->SendToAllClients(pBuffer, pBufferLen);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if(f_ClientConnected)
		{
			m_Client->SendData(pBuffer, pBufferLen);
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void NetworkingEngine::RecvData()
{
	if(f_ServerMode)
	{
		if(f_ServerIsBinded)
		{
			m_Server->HandlePackets(&m_Packets);
			if(m_Packets.empty() == false)
			{
				//Set the flag to true so that the DevilerPackets method can know who it goes to.
				f_DataReceived = true;
			}
		}
		else
		{
			//return 0;
		}
	}
	else
	{
		if(f_ClientConnected)
		{
			char clientBuffer[256];
			ZeroMemory(&clientBuffer, sizeof(clientBuffer));
			int bytes = m_Client->ReceiveData(clientBuffer);
			if(bytes > 0)
			{
				PacketHeader h;
				ZeroMemory(&h, sizeof(PacketHeader));
				memcpy(&h, &clientBuffer, sizeof(PacketHeader));

				if(h.m_Type > 0)
				{
					//On client mode the ID will be zero just make sure you remember that.
					char *temp = new char[bytes];
					memcpy(temp, clientBuffer, bytes);
					m_Packets.push(make_pair(m_Client->getID(), temp));
					//Set the flag to true so that the DevilerPackets method can know who it goes to.
					f_DataReceived = true;
				}
			}
		}
		else
		{
			//return 0;
		}
	}
}

void NetworkingEngine::SwitchToClient()
{
	delete m_Server;

	m_Client = new Client(0);

	f_ServerMode = false;
	f_ClientConnected = false;

	output("Engine has switched to client mode.");
}

void NetworkingEngine::SwitchToServer()
{
	delete m_Client;

	m_Server = new Server(0);

	f_ServerMode = true;
	f_ServerIsBinded = false;

	output("Engine has switched to server mode.");
}

NetworkingEngine::~NetworkingEngine()
{
	if(f_ServerMode)
	{
		delete m_Server;
	}
	else
	{
		delete m_Client;
	}
}