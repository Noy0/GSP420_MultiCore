#ifndef SOCKET_H
#define SOCKET_H

#include <winsock.h>
#include <iostream>
#include <iomanip>

#include "PacketPresets.h"
#include "TimerTool.h"

using namespace std;

//SET TO FALSE WHEN RUNNING IN A WINDOWS APP
#define TESTING false

//Couts any data passed into it.
#define output(x){ if(TESTING){cout << x << endl;}}
//Couts any WSA error with an error name and description.
#define error(msg, error){if(TESTING){cout << ("Error {0}: {1}", error, msg) << endl;}}
//Error checking tool that uses a macro to ouput the WSAGetLastError description and error name.
int EC(int pCheck);

struct SocketTransmissionStruct 
{
	SOCKET		skSocket;
	char		szBuffer[64000];
	int			iWritePos;
	int			iReadPos;
	int			iTerminate;
};

class Socket
{
public:
	Socket();
	~Socket();

	//Sets the socket to listen on the port it has binded to.
	int Listen();
	//Disconnects the socket from the connection.
	void Disconnect();
	//Binds the socket to the specified port.
	int Bind(int pPort);
	//Binds the socket to the specified port on UDP so it can broadcast. (connectionless mode)
	int BindForBroadcast(int pPort);
	//Accepts a client socket. Parameter takes a Socket class object.
	bool Accept(Socket* pAcceptSocket);
	//Connects to a server with the specified address and port.
	bool Connect(char* pServerAddress, int pPort);
	//Starts a receiving thread (Used mainly by the server).
	void StartRecvThread();

	//Checks a buffer to see if there is a packet, and pulls it out of the buffer. Also returns how many bytes were retrieved.
	int GetPacket(char *szBuffer);
	//Recieves data from the socket it is connected to and stores it in a character buffer.
	int Recv(char *szBuffer, int iBufLen);
	//Recieves data from a broadcast and stores it in a character buffer.
	int RecvBroadcast(char *szBuffer, int iBuflen);
	//Sends a buffer of data to the socket it is connected to.
	int Send(char *szBuffer, int iBufLen);
	//Sends a buffer of data as a broadcast.
	int SendBroadcast(char *szBuffer, int iBuflen);
	//A thread that is used to recieve data.
	static void thrReceiveThread(SocketTransmissionStruct *rs);
	//A thread used to send data.
	static void thrSendThread(SocketTransmissionStruct &rs);

private:

	//Buffer used to hold incoming data.
	SocketTransmissionStruct	stReceive;
	//Buffer used to hold data being sent.
	SocketTransmissionStruct	stSend;	
	//Handle to the recieve thread.
	DWORD						dwReceiveHandle;
	//Handle to the send thread.
	DWORD						dwSendHandle;
	//SOCKET object.
	SOCKET m_Socket;
	//sockaddr_in struct variable used to store the port and address.
	sockaddr_in m_Address;
};

#endif //SOCKET_H