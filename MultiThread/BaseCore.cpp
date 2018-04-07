#include "BaseCore.h"


unsigned __stdcall BaseCore::CoreThread(void* core)
{
	((BaseCore*)core)->m_Terminated = false;
	((BaseCore*)core)->Run();
	((BaseCore*)core)->m_Terminated = true;
	return 0;
}

bool BaseCore::HandleMessage(SMessage *message)
{
	WaitForSingleObject(m_MessageQueueMutex, INFINITE);
	m_MessageQueue.push(message);
	ReleaseMutex(m_MessageQueueMutex);
	return true;
}

void BaseCore::Run()
{
	OnLaunch();

	//Calculate sec per count
	__int64 countspersec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countspersec);
	float secpercount = 1.0f / (float)countspersec;

	//Get timestamp
	__int64 prevtimestamp;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevtimestamp);
	__int64 currenttimestamp;
	float dt;
	//float dt1[10] = {0};

	int counter = 0;
	while(!m_Quit)
	{
		//Update core
		counter ++;
		QueryPerformanceCounter((LARGE_INTEGER*)&currenttimestamp);
		dt = (float)(currenttimestamp - prevtimestamp) * secpercount;
		//dt1[counter] = dt;
		if(dt >= m_SecondsPerFrame)
		{
			counter = 0;
			prevtimestamp = currenttimestamp;
			m_LastSPF = dt;
			Update(dt);

		}
		else
		{
			Sleep(1);
		}
	}
}

void BaseCore::CopyMessage()
{
	//Relocate the messages using mutex
	WaitForSingleObject(m_MessageQueueMutex, INFINITE);
	while(!m_MessageQueue.empty())
	{
		m_QueueBuffer.push(m_MessageQueue.front());
		m_MessageQueue.pop();
	}
	ReleaseMutex(m_MessageQueueMutex);
}