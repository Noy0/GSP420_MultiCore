#pragma once

#include "..\\Message.h"
#include <string>
#include "..\\DDScriptData.h"

enum EScriptMessage{

	MSG_RUN_SCRIPT,
	MSG_REG_SDATA
};

struct SMessageRunScript : public SMessage{

	SMessageRunScript( std::string newScript ) : 
						SMessage( CORE_SCRIPTING, MSG_RUN_SCRIPT ), script(newScript)
	{}
	std::string script;
};

struct SMessageRegisterScriptObject : public SMessage{
	SMessageRegisterScriptObject(DD_ScriptData* NewSData): 
					SMessage(CORE_SCRIPTING,MSG_REG_SDATA), SData(NewSData){}

	DD_ScriptData* SData;
};