#ifndef SERVER_CLIENT_BASE_H
#define SERVER_CLIENT_BASE_H

#include "Socket.h"

class SCBase
{
public:
	SCBase();
	~SCBase();

	//Socket object used to perform all networking operations.
	Socket* m_Socket;

	//Returns the id given to this server/client.
	int getID();
	//Sets the id for this server/client. Recommend for use in inheriting class's constructor only.
	void setID(int pID);

private:
	//ID used to identify the socket holder within the program.
	int m_ID;
};

#endif //SERVER_CLIENT_BASE_H