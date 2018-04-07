#include "NetworkingCore.h"

NetCore::NetCore()
{
	m_Engine = new NetworkingEngine();
}

void NetCore::Update(float dt)
{
	CopyMessage();
	ExecuteMessage();
	m_Engine->RecvData();
	DeliverPackets();
}

void NetCore::ExecuteMessage()
{
	while(m_QueueBuffer.empty() == false)
	{
		SMessage* newMsg = m_QueueBuffer.front();
		switch(newMsg->Message)
		{
		case MSG_SWITCHTOCLIENT:
			{
				SMessageSwitchToClient* msg = (SMessageSwitchToClient*) newMsg;
				m_Engine->SwitchToClient();
				delete newMsg;
				break;
			}
		case MSG_SWITCHTOSERVER:
			{
				SMessageSwitchToServer* msg = (SMessageSwitchToServer*) newMsg;
				m_Engine->SwitchToServer();
				delete newMsg;
				break;
			}
		case MSG_SERVER_BIND:
			{
				SMessageServerBind* msg = (SMessageServerBind*) newMsg;
				m_Engine->s_Bind(msg->m_Port);
				delete newMsg;
				break;
			}

		case MSG_SERVER_UNBIND:
			{
				SMessageServerUnbind* msg = (SMessageServerUnbind*) newMsg;
				m_Engine->s_UnBind();
				delete newMsg;
				break;
			}
		case MSG_CLIENT_CONNECT:
			{
				SMessageClientConnect* msg = (SMessageClientConnect*) newMsg;
				m_Engine->c_Connect(msg->m_Address, msg->m_Port);
				delete newMsg;
				break;
			}
		case MSG_CLIENT_DISCONNECT:
			{
				SMessageClientDisconnect* msg = (SMessageClientDisconnect*) newMsg;
				m_Engine->c_Disconnect();
				delete newMsg;
				break;
			}
		case MSG_SENDDATA:
			{
				SMessageSendData* msg = (SMessageSendData*) newMsg;
				m_Engine->SendData(msg->m_Buffer, sizeof(msg->m_Buffer));
				delete newMsg;
				break;
			}
		case MSG_GETCLIENTCOUNT:
			{
				SMessageGetClientCount* msg = (SMessageGetClientCount*) newMsg;
				*(msg->Count) = m_Engine->m_Server->getClientCount();
			}
		default:
			break;
		}

		m_QueueBuffer.pop();

	}
}

void NetCore::DeliverPackets()
{
	while(m_Engine->m_Packets.empty() == false)
	{
		void* data = (void*) m_Engine->m_Packets.front().second;
		int size = sizeof(m_Engine->m_Packets.front().second);
		//int size = sizeof(data);
		gCoreMgr->HandleMessage( new SMessageReceivedFromNet(data, size) );
		m_Engine->m_Packets.pop();
	}
}

NetCore::~NetCore()
{
}