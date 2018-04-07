#ifndef PACKET_PRESETS_H
#define PACKET_PRESETS_H

//THIS HEADER MUST BE INCLUDED IN EVERY TYPE OF PACKET MADE AND FILLED OUT BEFORE THE PACKET IS SENT OUT.
struct PacketHeader
{
	int		m_Type;
	int		m_Length;
};

//These are the types of events there are for event packets.
#define SE_ECHO		1
#define SE_DISCON	2
//Event packets are type 1.
struct EventPacket
{
public:
	PacketHeader m_Header;
	int m_Event;
};

//Chat packets are type 2.
struct ChatPacket
{
public:
	PacketHeader m_Header;
	char m_Message[128];
};

#endif //PACKET_PRESETS_H