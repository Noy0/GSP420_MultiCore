#include "Debug.h"

CDebug::CDebug( string openFile){

	mOutFile.open( openFile.c_str() );

}

CDebug::~CDebug(){

	mOutFile.close();
}

void CDebug::Print( int line, Error err)
{

	if(line !=0)
		mOutFile <<"	Line "<< line;

	switch(err)
	{
	case PAREN_MISMATCH:
		mOutFile <<  ": Parentheses do not match\n";
		break;
	case ILLEGAL_CHAR:
		mOutFile <<  ": Illegal character\n";
		break;
	case VAR_NOT_FOUND:
		mOutFile <<  ": Identifier not found\n";
		break;
	case ILLEGAL_END_WHILE:
		mOutFile <<  ": Illegal end while\n";
		break;
	case NO_END_WHILE:
		mOutFile <<  ": No end while found\n";
		break;
	case ILLEGAL_END_IF:
		mOutFile <<  ": Illegal end if\n";
		break;
	case NO_END_IF:
		mOutFile <<  ": No end if found\n";
		break;
	case ILLEGAL_ELSE:
		mOutFile <<  ": Illegal else\n";
		break;
	case NO_OBJ_NAME:
		mOutFile <<  ": Missing object name\n";
		break;
	case NESTED_OBJ:
		mOutFile <<  ": Illegal nesting of objects\n";
		break;
	case NAME_NOT_NEEDED:
		mOutFile <<  ": Script name not needed\n";
		break;
	case NO_END_OBJ:
		mOutFile <<  ": No end object found\n";
		break;
	case NESTED_SCRIPT:
		mOutFile <<  ": Illegal nesting of scripts\n";
		break;
	case NO_SCRIPT_NAME:
		mOutFile <<  ": No script name found\n";
		break;
	case NO_END_SCRIPT:
		mOutFile <<  ": No end script found\n";
		break;
	case NESTED_EVENT:
		mOutFile <<  ": Illegal nesting of events\n";
		break;
	case NO_EVENT_NAME:
		mOutFile <<  ": No event name found\n";
		break;
	case NO_SUCH_EVENT:
		mOutFile <<  ": Event does not exist\n";
		break;
	case NO_END_EVENT:
		mOutFile <<  ": No end event found\n";
		break;
	case NONE:
		mOutFile <<  "	Error Free!  Good job!\n";
	}
	

	mOutFile.flush();
}

void CDebug::PrintScript(string script)
{
	mOutFile << script <<endl;

	mOutFile.flush();
}