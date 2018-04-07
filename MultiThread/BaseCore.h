#ifndef _BASECORE_H
#define _BASECORE_H

#include "Message.h"
#include <queue>
#include <windows.h>
#include <process.h>

using std::queue;
using std::pair;

class BaseCore
{
public:
	static unsigned __stdcall CoreThread(void* core);
	BaseCore() : m_Quit(false), m_Terminated(true), m_SecondsPerFrame(0)
	{
		m_MessageQueueMutex = CreateMutex(0, 0, 0);
	}
	virtual bool HandleMessage(SMessage* message);
	virtual void Update(float dt){}
	virtual void Quit(){m_Quit = true;}
	bool Terminated() {return m_Terminated;}
	void SetFrameTime(float seconds){m_SecondsPerFrame = seconds;}
	void SetFramesPerSecond(float framespersecond)
		{
			m_SecondsPerFrame = 1.0f / framespersecond;
		}
	float GetLastSPF(){return m_LastSPF;}
	virtual ~BaseCore(){}
protected:
	virtual void OnLaunch(){}
	virtual void Run();
	virtual void CopyMessage();

	queue<SMessage*> m_MessageQueue;
	HANDLE m_MessageQueueMutex;
	queue<SMessage*> m_QueueBuffer;

	bool m_Quit;
	bool m_Terminated;
private:
	float m_SecondsPerFrame;
	float m_LastSPF;
};

#endif