#ifndef DDSCRIPTDATA_H
#define DDSCRIPTDATA_H 

#include "DynamicData.h"
#include <string>

typedef struct ScriptData{
	int objID;
	std::string objType;
	int LMouseClick;
	int RMouseClick;
	int Collision;
	
}SCRIPTDATA;

class DD_ScriptData : public DynamicData<vector<ScriptData>>
{
public:
	DD_ScriptData():DynamicData("Scripting"){}
private:
};

#endif