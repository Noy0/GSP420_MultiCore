#include "ServerClientBase.h"

SCBase::SCBase()
{
	m_ID = 0;
	m_Socket = new Socket();
}

int SCBase::getID()
{
	return m_ID;
}

void SCBase::setID(int pID)
{
	m_ID = pID;
}

SCBase::~SCBase()
{
	//do something
}
