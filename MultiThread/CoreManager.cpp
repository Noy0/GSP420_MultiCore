#include "CoreManager.h"
#include "CoreList.h"
#include "SystemMessage.h"

CoreManager* gCoreMgr = 0;

//Create function creates the singleton instance and stores its reference to global pointer.
void CoreManager::Create()
{
	static CoreManager instance;
	gCoreMgr = &instance;
}

//Constructor
CoreManager::CoreManager()
{
	m_Launched = 0;
	timeBeginPeriod(3);
}

//AddCore function takes a core pointer and adds it to the map.
//Auto-resize the array and fills the gap with NULL(0)
void CoreManager::AddCore(BaseCore *pCore, ECore nID)
{
	//If the vector memory has been allocated the replace the pointer
	if((int)nID < m_vCoreMap.size())
	{
		if(m_vCoreMap[(int)nID])
		{
			delete m_vCoreMap[(int)nID];
		}
		m_vCoreMap[(int)nID] = pCore;
	}
	//Or allocate the vector and add in the pointer, fill gap with 0.
	else
	{
		int oldsize = m_vCoreMap.size();
		m_vCoreMap.resize((int)nID + 1);
		for(int i = oldsize; i < m_vCoreMap.size() - 1; ++i)
		{
			m_vCoreMap[i] = 0;
		}
		m_vCoreMap[(int)nID] = pCore;
	}
}

//RemoveCore function removes one element of core map and delete the data
void CoreManager::RemoveCore(ECore nID)
{
	if(m_vCoreMap.size() - 1 > nID)
	{
		delete m_vCoreMap[(int)nID];
		m_vCoreMap[(int)nID] = 0;
	}
}

//Sets frames per second for a core
void CoreManager::SetCoreFramesPerSecond(ECore CoreID, float fps)
{
	if(CoreID > m_vCoreMap.size() || m_vCoreMap[CoreID] == 0)
		return;
	m_vCoreMap[CoreID]->SetFramesPerSecond(fps);
}

float CoreManager::GetLastSPF(ECore CoreID)
{
	if(CoreID > m_vCoreMap.size() || m_vCoreMap[CoreID] == 0)
		return 0;
	return m_vCoreMap[CoreID]->GetLastSPF();
}

//Message handler
bool CoreManager::HandleMessage(SMessage *pMessage)
{
	//if the reciever is CORE_NULL, then pass the message to all cores
	if(pMessage->Receiver == CORE_NULL)
	{
		switch(pMessage->Message)
		{
		case MSG_ENDAPP:
			{
				TerminateAll();
				while(!AllStopped())
				{
					Sleep(1);
				}
			}
		}
		delete pMessage;
		return true;
	}
	else
	{
		//Pass the message to receiver, check if valid
		if(m_vCoreMap.size() > pMessage->Receiver && m_vCoreMap[pMessage->Receiver])
		{
			if(m_vCoreMap[pMessage->Receiver]->HandleMessage(pMessage))
			{
				return true;
			}
			else
			{
				if(pMessage)
				{
					delete pMessage;
				}
				return false;
			}
		}
		else
		{
			delete pMessage;
			return false;
		}
	}
}


void CoreManager::Launch()
{
	for(int i = 0; i < m_vCoreMap.size(); ++i)
	{
		if(m_vCoreMap[i])
			_beginthreadex(0, 0, BaseCore::CoreThread, m_vCoreMap[i], 0, 0);
	}
}

void CoreManager::Update(float dt)
{
	for(int i = 0; i < m_vCoreMap.size(); ++i)
	{
		if(m_vCoreMap[i])
			m_vCoreMap[i]->Update(dt);
	}
}

void CoreManager::Terminate(ECore nID)
{
	if(nID > m_vCoreMap.size() - 1 && m_vCoreMap[nID])
	{
		m_vCoreMap[nID]->Quit();
	}
}

void CoreManager::TerminateAll()
{
	for(int i = 0; i < m_vCoreMap.size(); ++i)
	{
		if(m_vCoreMap[i])
			m_vCoreMap[i]->Quit();
	}
}

bool CoreManager::AllStopped()
{
	for(int i = 0; i < m_vCoreMap.size(); ++i)
	{
		if(m_vCoreMap[i] && !m_vCoreMap[i]->Terminated())
			return false;
	}
	return true;
}

//Destructor, destroy all cores in map
CoreManager::~CoreManager()
{
	for(int i = 0; i < m_vCoreMap.size(); ++i)
	{
		if(m_vCoreMap[i])
			delete m_vCoreMap[i];
	}
	timeEndPeriod(3);
}