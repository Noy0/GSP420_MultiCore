#ifndef SCRIPTUTIL_H
#define SCRIPTUTIL_H

#include <vector>
#include <map>
#include <string>
#include <stack>
#include <sstream>
#include <typeinfo>
#include <fstream>


using std::map;
using std::vector;
using std::pair;
using std::string;
using std::stack;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::endl;


typedef pair< string, pair<string, void* > > VARIABLE;
typedef int (*FUNC)();
typedef int WRAPPER;

#define IF    0
#define WHILE 1

#define GLOBAL 0
#define LOCAL 1

#define SLEEP_TIME 0.1

extern stringstream Stream;

enum{
	DEFAULT, 
	SEMI,
	OPEN_PAR,
	CLOSE_PAR,
	COMMA,
	ASTERISK,
	BACK_SLASH, 
	MOD, 
	PLUS, 
	MINUS,
	EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	GREATER_THAN,
	LESS_OR_EQUAL_TO,
	GREATER_OR_EQUAL_TO,
	AND,
	OR,
	NOT
};

enum Error{
	NONE,
	PAREN_MISMATCH,
	VAR_NOT_FOUND,
	ILLEGAL_CHAR,
	ILLEGAL_END_WHILE,
	NO_END_WHILE,
	ILLEGAL_END_IF,
	NO_END_IF,
	ILLEGAL_ELSE,
	NO_OBJ_NAME,
	NESTED_OBJ,
	NAME_NOT_NEEDED,
	NO_END_OBJ,
	NESTED_SCRIPT,
	NO_SCRIPT_NAME,
	NO_END_SCRIPT,
	NESTED_EVENT,
	NO_EVENT_NAME,
	NO_SUCH_EVENT,
	NO_END_EVENT
};

enum EEvent{
	ERR,
	LEFT_CLICK,
	RIGHT_CLICK,
	COLLISION
};

typedef struct SFlags{

	int LMouseClick;
	int RMouseClick;
	int Collision;

	SFlags(){

		LMouseClick = 0;
		RMouseClick = 0;
		Collision   = 0;
	}
}FLAGS;

enum ENodeType{

	NTYPE_VAR_CREATE,
	NTYPE_VAR_ASSIGNMENT,
	NTYPE_FUNCTION_CALL,
	NTYPE_IF_BEGIN,
	NTYPE_IF_END,
	NTYPE_WHILE_BEGIN,
	NTYPE_WHILE_END,
	NTYPE_ELSE
};



#endif