#ifndef NETWORKING_CORE_H
#define NETWORKING_CORE_H

#include "NetworkingEngine.h"
#include "NetCoreMessage.h"
#include "..\\BaseCore.h"
#include "..\\CoreManager.h"
#include "..\\GameMessage.h"

class NetCore : public BaseCore
{
public:
	NetCore();
	~NetCore();

	void Update(float dt);
	void DeliverPackets();

private:
	void ExecuteMessage();
	NetworkingEngine* m_Engine;
};

#endif //NETWORKING_CORE_H