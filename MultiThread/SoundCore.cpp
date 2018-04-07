#include "SoundCore.h"
#include "SoundMessage.h"


SoundCore::SoundCore(HWND handle):mSoundEng(handle)
{
}
SoundCore::~SoundCore()
{
}

void SoundCore::Update(float dt)
{
	CopyMessage();
	HandleSoundMessage();
}

void SoundCore::HandleSoundMessage()
{
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_PLAY:
			{
				SMessagePlay* msgex = (SMessagePlay*)msg;
				mSoundEng.Play(msgex->song, msgex->loop);
				delete msg;
				break;
			}
			case MSG_STOP:
			{
				SMessageStop* msgex = (SMessageStop*)msg;
				mSoundEng.Stop(msgex->song);
				delete msg;
				break;
			}
			case MSG_SET_VOL:
			{
				SMessageSetVol* msgex = (SMessageSetVol*)msg;
				mSoundEng.SetVolume(msgex->song, msgex->Vol);
				delete msg;
				break;
			}
			default:
			{
				
				break;
			}
		}
		m_QueueBuffer.pop();
	}
}