#include "Socket.h"

Socket::Socket()
{
	WSADATA wsaData;
	WORD winsockVersion;

	//Using winSock2.h
	winsockVersion = MAKEWORD(2, 0);

	m_Socket = INVALID_SOCKET;
	WSAStartup(winsockVersion, &wsaData);
}

int Socket::Bind(int pPort)
{
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Initialize server to 0;
	memset(&m_Address, 0, sizeof(sockaddr_in));

	m_Address.sin_family = AF_INET;
	m_Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_Address.sin_port = htons(pPort);

	return bind(m_Socket, (sockaddr*) &m_Address, sizeof(sockaddr_in));
}

int Socket::BindForBroadcast(int pPort)
{
	m_Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Initialize server to 0.
	memset(&m_Address, 0, sizeof(sockaddr_in));

	m_Address.sin_family = PF_INET;
	m_Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_Address.sin_port = htons(pPort);

	char broadcast = '1';

	if(setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	else
	{
		return bind(m_Socket, (sockaddr*) &m_Address, sizeof(sockaddr_in));
	}
}

int Socket::Listen()
{
	return listen(m_Socket, 20);
}

bool Socket::Connect(char* pServerAddress, int pPort)
{
	sockaddr_in server;
	LPHOSTENT host;

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_Socket == INVALID_SOCKET)
	{
		return false;
	}

	memset(&server, 0, sizeof(sockaddr_in));
	server.sin_family = AF_INET;

	//Attempt to set the address using the parameter given.
	server.sin_addr.S_un.S_addr = inet_addr(pServerAddress);
	if(server.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		//If the condition above is true then the pServerAddress was
		//in hostname format instead of dot format, so convert it.
		host = gethostbyname(pServerAddress);

		//If the host is null then the pServerAddress doesn't exist
		//and false will be returned.
		if(host == NULL)
		{
			return false;
		}
		else
		{
			//Otherwise, get the first address from the address list and set
			//it as the server's address.
			server.sin_addr.S_un.S_addr = ((LPIN_ADDR)host->h_addr_list)->S_un.S_addr;
		}
	}
	//Set the port to which the client will connect.
	server.sin_port = htons(pPort);

	int error = connect(m_Socket, (sockaddr*)&server, sizeof(sockaddr_in));
	if( error == SOCKET_ERROR)
	{
		Disconnect();
		return false;
	}

	StartRecvThread();

	return true;
}

bool Socket::Accept(Socket* pAcceptSocket)
{
	sockaddr_in client;
	int clientSize = sizeof(sockaddr_in);

	pAcceptSocket->m_Socket = accept(m_Socket, (sockaddr*) &client, &clientSize);

	//If there's an error, return false, otherwise return true.
	if(pAcceptSocket->m_Socket == INVALID_SOCKET)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Socket::StartRecvThread()
{
	ZeroMemory(&stReceive, sizeof(stReceive));
	stReceive.skSocket = m_Socket;
	// Create the thread to receive data
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) &thrReceiveThread, &stReceive, NULL, &dwReceiveHandle);
}

void Socket::Disconnect()
{
	if(m_Socket != INVALID_SOCKET)
	{
		//CLose the socket and set it to 0
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	else
	{
		//Then the socket wasn't already opened.
	}
}

int Socket::Recv(char *szBuffer, int iBufLen)
{
	return recv(m_Socket, szBuffer, iBufLen, NULL);
}

int Socket::RecvBroadcast(char* szBuffer, int iBuflen)
{
	int len = sizeof(struct sockaddr_in);
	return recvfrom(m_Socket, szBuffer, iBuflen, 0, (sockaddr*) &m_Address, &len);
}

int Socket::Send(char *szBuffer, int iBufLen)
{
	return send(m_Socket, szBuffer, iBufLen, NULL);
}

int Socket::SendBroadcast(char *szBuffer, int iBuflen)
{
	return sendto(m_Socket, szBuffer, iBuflen, 0,(sockaddr*) &m_Address, sizeof(struct sockaddr_in));
}

void Socket::thrReceiveThread(SocketTransmissionStruct *rs)
{
	int				iBytesReceived;
	char			*szTempBuffer;
	int				iBytesPart1;
	int				iBytesPart2;

	szTempBuffer = new char[32768];

	// Receive data until given notice to terminate
	while( rs->iTerminate != 1 ) 
	{
		// Read from the pipe
		iBytesReceived = recv( rs->skSocket, szTempBuffer, 32768, 0 );
		if( iBytesReceived > 0 ) 
		{
			// Make sure the packet does not overrun the write buffer
			if( (rs->iWritePos+iBytesReceived) >= 64000 ) {
				iBytesPart1 = ((rs->iWritePos+iBytesReceived)-64000);
				iBytesPart2 = (64000 - rs->iWritePos);
				memcpy( &rs->szBuffer[rs->iWritePos], szTempBuffer, iBytesPart1 );
				memcpy( &rs->szBuffer[0], &szTempBuffer[iBytesPart1], iBytesPart2 );
				rs->iWritePos = iBytesPart2;
			}
			else {
				// Write to the permanent buffer
				memcpy( &rs->szBuffer[rs->iWritePos], szTempBuffer, iBytesReceived );
				rs->iWritePos += iBytesReceived;
			}
		}
		else
		{
			if(iBytesReceived == 0)
			{
				//DO SOMETHING TO TELL THE USER THAT THE CONNECTION HAS BEEN GRACEFULLY CLOSED.
				rs->iTerminate = 1;
			}
		}
	}

	delete [] szTempBuffer;
}
void Socket::thrSendThread(SocketTransmissionStruct &rs)
{
	send( rs.skSocket, rs.szBuffer, 128, 0 );
}

int Socket::GetPacket(char *szBuffer)
{
	int				iBytesReceived = 0;
	int				iBytesWaiting = 0;
	PacketHeader	stHeader;

	if(stReceive.iTerminate == 1 && stReceive.iWritePos == stReceive.iReadPos)
	{
		return -1;
	}
	else
	{
		// Check if write pos moved
		if( stReceive.iWritePos != stReceive.iReadPos )
		{
			//
			// Pull packet header from buffer
			//
			iBytesWaiting = (stReceive.iWritePos - stReceive.iReadPos);
			// Make sure a full size header is present
			if( iBytesWaiting < sizeof(stHeader) ) 
			{
				return(0);
			}
			// Copy the header in
			memcpy(&stHeader,&stReceive.szBuffer[stReceive.iReadPos],sizeof(stHeader));
			//
			// Pull the body of the packet according to the size 
			//
			
			// Make sure enough data is waiting, if not leave and try again later
			if( (iBytesWaiting-sizeof(stHeader)) < (unsigned)stHeader.m_Length )
			{
				return(0);
			}
			// Copy into the return buffer
			memcpy(szBuffer,&stHeader,sizeof(stHeader));
			memcpy(&szBuffer[sizeof(stHeader)],&stReceive.szBuffer[stReceive.iReadPos+sizeof(stHeader)],stHeader.m_Length);

			// Update Read Position & Return Values
			stReceive.iReadPos += (stHeader.m_Length+sizeof(stHeader));
			iBytesReceived = (stHeader.m_Length+sizeof(stHeader));
			
			// Check if reading too far
			if( stReceive.iReadPos >= 64000 )
			{
				stReceive.iReadPos = 0;
			}
		}
	}

	return(iBytesReceived);
}

Socket::~Socket()
{
	CloseHandle(&dwReceiveHandle);
	Disconnect();
}

int EC(int pCheck)
{
	if(pCheck == -1)
	{
		switch(WSAGetLastError())
		{
		case WSAEINTR:
			error("A blocking operation was interrupted by a call to WSACancelBlockingCall.", "WSAEINTR");
			return WSAEINTR;
		case WSAEFAULT:
			error("The system detected an invalid pointer address in attempting to use a pointer argument in a call.", "WSAEFAULT");
			return WSAEFAULT;
		case WSAEINVAL:
			error("An invalid argument was supplied.", "WSAEINVAL");
		case WSAEMFILE:
			error("Too many open sockets.", "WSAEMFILE");
			return WSAEMFILE;
		case WSAEWOULDBLOCK:
			error("A non-blocking socket operation could not be completed immediately.", "WSAEWOULDBLOCK");
			return WSAEWOULDBLOCK;
		case WSAEINPROGRESS:
			error("A blocking operation is currently executing.", "WSAEINPROGRESS");
			return WSAEINPROGRESS;
		case WSAEADDRNOTAVAIL:
			error("The requested address is not valid in its context.", "WSAEADDRNOTAVAIL");
			return WSAEADDRNOTAVAIL;
		case WSAENETDOWN:
			error("A socket operation encountered a dead network.", "WSAENETDOWN");
			return WSAENETDOWN;
		case WSAENETUNREACH:
			error("A socket operation was attempted to an unreachable network.", "WSAENETUNREACH");
			return WSAENETUNREACH;
		case WSAECONNABORTED:
			error("An established connection was aborted by the software in your host machine.","WSAECONNABORTED");
			return WSAECONNABORTED;
		case WSAECONNRESET:
			error("An existing connection was forcibly closed by the remote host.","WSAECONNRESET");
			return WSAECONNRESET;
		case WSAEISCONN:
			error("A connect request was made on an already connected socket.","WSAEISCONN");
			return WSAEISCONN;
		case WSAENOTCONN:
			error("A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.","WSAENOTCONN");
			return WSAENOTCONN;
		case WSAESHUTDOWN:
			error("A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.","WSAESHUTDOWN");
			return WSAESHUTDOWN;
		case WSAETIMEDOUT:
			error("A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.","WSAETIMEDOUT");
			return WSAETIMEDOUT;
		case WSAECONNREFUSED:
			error("No connection could be made because the target machine actively refused it.","WSAECONNREFUSED");
			return WSAECONNREFUSED;
		case WSAELOOP:
			error("Cannot translate name.","WSAELOOP");
			return WSAELOOP;
		case WSAEHOSTDOWN:
			error("A socket operation failed because the destination host was down.","WSAEHOSTDOWN");
			return WSAEHOSTDOWN;
		case WSAEHOSTUNREACH:
			error("A socket operation was attempted to an unreachable host.","WSAEHOSTUNREACH");
			return WSAEHOSTUNREACH;
		case WSASYSNOTREADY:
			error("WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.","WSASYSNOTREADY");
			return WSASYSNOTREADY;
		case WSAVERNOTSUPPORTED:
			error("The Windows Sockets version requested is not supported.","WSAVERNOTSUPPORTED");
			return WSAVERNOTSUPPORTED;
		case WSANOTINITIALISED:
			error("Either the application has not called WSAStartup, or WSAStartup failed.","WSANOTINITIALISED");
			return WSANOTINITIALISED;
		case WSAEDISCON:
			error("Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.","WSAEDISCON");
			return WSAEDISCON;
		/*case :
			error("","");
			return ;*/
		default:
			cout << "Error: Description unavailable." << endl;
			return -1;
		}
	}
	else
	{
		return pCheck;
	}
}