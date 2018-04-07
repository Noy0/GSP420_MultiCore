#include "Message.h"

enum ESystemMessage
{
	MSG_ENDAPP
};

struct SMessageEndApp : public SMessage
{
	SMessageEndApp() : SMessage(CORE_NULL, MSG_ENDAPP)
	{}
};