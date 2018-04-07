#pragma once
#include "..\\ScriptUtil.h"

class CDebug{

public:
	CDebug( string openFile );
	~CDebug();

	void Print( int, Error );
	void PrintScript(string);

private:
	ofstream mOutFile;
};