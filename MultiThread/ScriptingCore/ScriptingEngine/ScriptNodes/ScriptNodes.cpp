#include "ScriptNodes.h"
//======debug======
#include <iostream>
using std::cout;
using std::endl;
//=================

stringstream Stream(stringstream::in | stringstream::out);

int CBaseNode::ConditionDelimiter(string str, int &pos)
{
	if(SearchForOperator(str,'|',pos)&& (str[pos+1] == '|'))
	{
		return OR;
	}

	if(SearchForOperator(str,'&',pos)&& (str[pos+1] == '&'))
	{
		return AND;
	}


	pos = (str.find_first_of("<>!="));

	if(pos != string::npos)
	{
		switch(str[pos])
		{
		case '<':
			if(str[pos+1] == '=')
			{
				return LESS_OR_EQUAL_TO;
			}

			else
			{
				return LESS_THAN;
			}
		
		case '>':
			if(str[pos+1] == '=')
			{
				return GREATER_OR_EQUAL_TO;
			}

			else
			{
				return GREATER_THAN;
			}
		//case '&':
		//	if(str[pos+1] == '&')
		//	{
		//		return AND;
		//	}
		//	break;

		//case '|':
		//	if(str[pos+1] == '|')
		//	{
		//		return OR;
		//	}
		//	break;

		case '=':
			if(str[pos+1] == '=')
			{
				return EQUAL;
			}
			break;
		case '!':
			if(str[pos+1] == '=')
			{
				return NOT_EQUAL;
			}
			break;

		}
	}

	if(str[0] == '!')
	{
		return NOT;
	}

	return DEFAULT;
}

int CBaseNode::GetDelimiter(string str, int &pos)
{
//	size_t delType;
//	size_t endPos;

	pos = (size_t)(str.find_first_of(";(),*/%+-"));

	if(pos != string::npos)		// improved
	{
		switch(str[pos])
		{
		case ';':
			return SEMI;
		case '(':
			return OPEN_PAR;
		case ')':
			return CLOSE_PAR;
		case ',':
			return COMMA;
		case '*':
			return ASTERISK;
		case '/':
			return BACK_SLASH;
		case '%':
			return MOD;
		case '+':
			return PLUS;
		case '-':
			return MINUS;

		}
	}

	return DEFAULT;
}

int CBaseNode::MatchParentheses(string str)
{
	int match = 1;
	int found = string::npos;
	for(int i = 0; i < str.size(); i++)
	{
		if(str[i] == '(')
		{
			match++;
		}

		else if(str[i] == ')')
		{
			match--;
		}

		if(match == 0)
		{
			found = i;
			break;
		}
	}

	return found;
}

bool CBaseNode::InParentheses(string str, int pos)
{
	//bool In = false;
	int close;
	for(int i = pos; i >= 0; i--)
	{
		if(str[i] == '(')
		{
			close = MatchParentheses(str.substr(i+1))+i+1;
			if(close > pos)
			{
				return true;
			}
		}
	}
	return false;
}

bool CBaseNode::SearchForOperator(string str, char ch, int& pos)
{
	//bool found;
	for(pos = 0; pos < str.size(); pos++)
	{
		if(str[pos] == ch)
		{
			if(!InParentheses(str, pos))
			{
				return true;
			}
		}
	}

	pos = string::npos;
	return false;
}

template <class Type>
Type CBaseNode::EvalExp(string script, string exp)
{
	int delPos;	//delimiter position
	int delType;
	int openPar;
	int closePar = -1;

	string Paren = "";
	string curExp;
	string nextExp;
	bool func = false;
	

	if(exp.find_last_of(";") != string::npos)
	{
		for(int i = 0;i < exp.size();i++)
		{
			if((exp[i] == ';'))
			{
				exp.erase(i,1);
				i--;
			}
		}
	}
	delType = GetDelimiter(exp,delPos);

	curExp = exp.substr(0,delPos);

	

	openPar = exp.find_first_of("(");

	if(openPar != string::npos)
	{
		closePar = MatchParentheses(exp.substr(openPar+1)) + openPar+1;
		Paren = exp.substr(openPar+1, closePar - openPar - 1);
	}
	

	if(exp == "")
	{
		return 0;
	}
	
	//else if((exp[0] == '(') &&(exp[exp.size()-2] == ')'))
	//{
	//	return EvalExp<Type>(script, exp.substr(1,exp.size()-3));
	//}

	
//if((exp.find_first_of("+") != string::npos) && !InParentheses(exp, exp.find_first_of("+")))
	if(SearchForOperator(exp,'+',delPos))
	{
		//delPos = exp.find_first_of("+");
		curExp = exp.substr(0,delPos);

		return EvalExp<Type>(script,curExp) + EvalExp<Type>(script,exp.substr(delPos+1));
	}

	if(SearchForOperator(exp,'-',delPos))
	{
		//delPos = exp.find_first_of("-");
		curExp = exp.substr(0,delPos);

		return EvalExp<Type>(script,curExp) - EvalExp<Type>(script,exp.substr(delPos+1));
	}

	if(SearchForOperator(exp,'*',delPos))
	{
		//delPos = exp.find_first_of("*");
		curExp = exp.substr(0,delPos);

		return EvalExp<Type>(script,curExp) * EvalExp<Type>(script,exp.substr(delPos+1));
	}

	if(SearchForOperator(exp,'/',delPos))
	{
		//delPos = exp.find_first_of("/");
		curExp = exp.substr(0,delPos);

		return EvalExp<Type>(script,curExp) / EvalExp<Type>(script,exp.substr(delPos+1));
	}
	
	if(SearchForOperator(exp,'%',delPos))
	{
		//delPos = exp.find_first_of("%");
		curExp = exp.substr(0,delPos);

		if("int" == typeid(Type).name())
		{
			return EvalExp<int>(script,curExp) % EvalExp<int>(script,exp.substr(delPos+1));
		}
	}
	
	if(openPar != string::npos)
	{
		delType = GetDelimiter(exp,delPos);

		curExp = exp.substr(0,delPos);
		
		if((curExp != "") && (FuncMgr->FindFunction(curExp)))
		{
			func = true;
			int endPos = exp.find_last_of(")");
			if(endPos != string::npos)
			{
				int delPos2;
				string temp;
				//string params = exp.substr(delPos+1, endPos - delPos - 1);
				//cout<<params<<endl;
				
				//delPos2 = params.find(',');
				delPos2 = Paren.find(',');
				if( delPos2 != string::npos )
				{

					while( delPos2 != string::npos )
					{

						temp = Paren.substr( 0, delPos2 );

						VarMgr->PushToStack(EvalExp<Type>(script,temp));

						Paren = Paren.substr( delPos2 + 1 );

						delPos2 = Paren.find( ',' );

						if( delPos2 == string::npos )
							VarMgr->PushToStack(EvalExp<Type>(script,Paren));

					}
				}
				else
				{
					VarMgr->PushToStack(EvalExp<Type>(script,Paren));
				}
			
				FuncMgr->GetFunction(curExp)();

				Type FuncType = VarMgr->StackToFloat(0);
				//delPos2 = VarMgr->StackToInt(0);

				VarMgr->ClearStack();

				return FuncType;//delPos2;
			}
		}
	}
	if(!func)
	{
		curExp = exp.substr(0,exp.find_first_of(";)"));
		if(VarMgr->GetVarByName(script, curExp) != NULL)
		{
			//return (int)vars[curExp].second;
			
			if("int" == VarMgr->GetVarByName(script, curExp)->second.first)
			{
				return (int) VarMgr->GetVarByName(script, curExp)->second.second;
			}

			else if("float" == VarMgr->GetVarByName(script, curExp)->second.first)
			{
				return *((float*)VarMgr->GetVarByName(script, curExp)->second.second);
			}

		}
		else if(Paren != "")
		{
			return EvalExp<Type>(script, Paren);
		}
		else
		{
			stringstream s(stringstream::in | stringstream::out);
			Type temp;
			s<<curExp;
			s>>temp;

			return temp;//atoi(curExp.c_str());
		}
	}

	return NULL;
}


////=======================================
//// CVariableNode functions
////=======================================
CVariableNode::CVariableNode(){

	//assignment = new VARIABLE();
	created = false;
}
CVariableNode::~CVariableNode(){

	//if( assignment->second.second ){

	//	delete assignment->second.second;
	//	assignment->second.second = 0;
	//}

	//if( assignment ){

	//	delete assignment;
	//	assignment = 0;
	//}

	//if( value ){

	//	delete this->value;
	//	value = 0;
	//}
}

CBaseNode* CVariableNode::Execute(){

	//TODO

	if( !created ){

		if(varType == "int")
		{
			int intVar = EvalExp<int>(Owner,expression);
			VarMgr->RegisterVariable(Owner,VarMgr->MakeIntVar(varName,intVar),scope);

			//cout<<"Int Var: "<<intVar<<endl;
		}

		else if(varType == "float")
		{
			float floatVar = EvalExp<float>(Owner,expression);
			VarMgr->RegisterVariable(Owner,VarMgr->MakeFloatVar(varName,floatVar),scope);

			//cout<<"Float Var: "<<floatVar<<endl;

		}

		else if(varType == "string")
		{
			//char* _string = EvalExp<char*>(Owner, expression);
			//VarMgr->RegisterVariable( Owner, VarMgr->MakeStringVar(varName, expression), 0);

			//cout << "String var: " << expression << endl;
		}

		created = true;
	}

	return this->next;
}

////=======================================
//// CAssignmentNode functions
////=======================================
CAssignmentNode::CAssignmentNode(){

	//assignment = new VARIABLE();
}
CAssignmentNode::~CAssignmentNode(){

	/*if( assignment->second.second ){

		delete assignment->second.second;
		assignment->second.second = 0;
	}

	if( assignment ){

		delete assignment;
		assignment = 0;
	}*/

	//if( value ){

	//	delete this->value;
	//	value = 0;
	//}

	//delete floatVar;
	//floatVar = 0;
}

CBaseNode* CAssignmentNode::Execute(){

	//TODO

	//VARIABLE* temp = new VARIABLE();
	//temp = VarMgr->GetVarByName(Owner,varName);
	//int intVar = EvalExpInt(Owner,expression);
	//if(varType == "int")
	//{
	//	
	//	//VarMgr->RegisterVariable(Owner,VarMgr->MakeIntVar(varName,intVar),0);
	//	VarMgr->AssignVariable(Owner,varName, VarMgr->MakeIntVar(varName,intVar));

	//	cout<<"assign intVar: "<<intVar<<endl;

	//}

	if(VarMgr->GetVarByName(Owner,varName) != NULL)
	{
		if(VarMgr->GetVarByName(Owner,varName)->second.first == "int")
		{
			//int intVar = EvalExp<int>(Owner,expression);
			VarMgr->GetVarByName(Owner,varName)->second.second = (int*)EvalExp<int>(Owner,expression);
		//	int bob = *((int*)VarMgr->GetVarByName(Owner,varName)->second.second);
		//	bob;
			//cout<<"assign intVar: "<<(int)VarMgr->GetVarByName(Owner,varName)->second.second<<endl;
		}

		else if(VarMgr->GetVarByName(Owner,varName)->second.first == "float")
		{
			//float floatVar = EvalExp<float>(Owner,expression);
			*((float*)VarMgr->GetVarByName(Owner,varName)->second.second) = EvalExp<float>(Owner,expression);
			//cout<<"assign floatVar: "<<*((float*)VarMgr->GetVarByName(Owner,varName)->second.second)<<endl;
		}

		//else if(temp->second.first == "string")
		//{
		//	char* stringVar = EvalExp<char*>(Owner,expression);
		//	temp->second.second = stringVar;
		//	cout<<"assign stringVar: "<<stringVar<<endl;
		//}

	}
	return this->next;
}
//
////========================
//// CFunctionNode functions
////========================
CFunctionNode::CFunctionNode(){
}

CFunctionNode::~CFunctionNode(){

	/*for( unsigned int i = 0;i < args.size();i++ ){

		if( args[i]->second.second ){

			delete args[i]->second.second;
			args[i]->second.second = 0;
		}

		if( args[i] ){

			delete args[i];
			args[i] = 0;
		}
	}

	args.clear();*/
}

CBaseNode* CFunctionNode::Execute(){

	//TODO
	//Place args onto stack in variable manager


	for(int i = 0; i < (int)args.size(); i++)
	{
		if((args[i].find_first_of("+-*/%") != string::npos) && (args[i].find('-') != 0))
		{

			VarMgr->PushToStack(EvalExp<float>(Owner,args[i]));
		}
		else if(args[i].find_first_of("-0123456789") != 0)
		{
			
			string type;
			
			type = VarMgr->GetVarByName(Owner,args[i])->second.first;

			if(type == "int")
			{
				VarMgr->PushToStack(EvalExp<int>(Owner,args[i]));
			}

			else if(type == "float")
			{
				VarMgr->PushToStack(EvalExp<float>(Owner,args[i]));
			}
		}
		else if(args[i].find(".") != string::npos)
		{
			VarMgr->PushToStack(EvalExp<float>(Owner,args[i]));
		}
		else
		{
			VarMgr->PushToStack(EvalExp<int>(Owner,args[i]));
		}
	}
	
	if( Function() != NULL )
		this->Function();

	VarMgr->ClearStack();
	return this->next;
}

//=====================
// CIfEndNode functions
//=====================
CBaseNode* CIfEndNode::Execute(){

	return this->next;
}

//====================
// CElseNode functions
//====================
CElseNode::CElseNode(){

	didIf = false;
}

CElseNode::~CElseNode(){

	if( ifEnd ){

		//delete ifEnd;
		//ifEnd = 0;
	}
}

CBaseNode* CElseNode::Execute(){

	if( didIf ){

		return this->ifEnd->next;	
	}
	else{

		return this->next;
	}
}

//========================
// CIfBeginNode functions
//========================
CIfBeginNode::~CIfBeginNode(){

	if( ifEnd ){

		//delete ifEnd;
		//ifEnd = NULL;
	}

	if( ifElse ){
//
		//delete ifElse;
		//ifElse = NULL;
	}
}

CBaseNode* CIfBeginNode::Execute(){

	if( Conditional( condition ) ){
		//cout<<"If true: \n";
		if( ifElse != NULL )
			this->ifElse->didIf = true;

		return this->next;
	}
	else{
		//cout << "If false: \n";
		if( ifElse != NULL ){

			return this->ifElse->next;
		}
		else{

			return ifEnd->next;
		}
	}

	return NULL;
}

bool CIfBeginNode::Conditional(std::string condition){

	
	if((condition[0] == '(') && ((condition.size() - 1) ==  (MatchParentheses(condition.substr(1)) + 1)))
	{	
		condition = condition.substr(1,condition.size() - 2);
	}
	int delPos;
	int delType = ConditionDelimiter(condition,delPos);

	string curExp = condition.substr(0,delPos);

	switch(delType)
	{
	case EQUAL:
		return EvalExp<float>(Owner, curExp) == EvalExp<float>(Owner,condition.substr(delPos+2));
	case NOT_EQUAL:
		return EvalExp<float>(Owner, curExp) != EvalExp<float>(Owner,condition.substr(delPos+2));
	case LESS_THAN:
		return EvalExp<float>(Owner, curExp) < EvalExp<float>(Owner,condition.substr(delPos+1));
	case LESS_OR_EQUAL_TO:
		return EvalExp<float>(Owner, curExp) <= EvalExp<float>(Owner,condition.substr(delPos+2));
	case GREATER_THAN:
		return EvalExp<float>(Owner, curExp) > EvalExp<float>(Owner,condition.substr(delPos+1));
	case GREATER_OR_EQUAL_TO:
		return EvalExp<float>(Owner, curExp) >= EvalExp<float>(Owner,condition.substr(delPos+2));
	case AND:
		return Conditional(curExp) && Conditional(condition.substr(delPos+2));
		//return EvalExp<int>(Owner, curExp) && EvalExp<int>(Owner,condition.substr(delPos+2));
	case OR:
		return Conditional(curExp) || Conditional(condition.substr(delPos+2));
		//return EvalExp<int>(Owner, curExp) || EvalExp<int>(Owner,condition.substr(delPos+2));
	case NOT:
		return !(Conditional(condition.substr(1)));
	default:
		return EvalExp<float>(Owner, curExp);
	}
	

	return false;
}

//========================
// CWhileEndNode functions
//========================
CWhileEndNode::~CWhileEndNode(){

	if( whileBegin ){

		//delete whileBegin;
		//whileBegin = 0;
	}
}

CBaseNode* CWhileEndNode::Execute(){

	return reinterpret_cast<CBaseNode*>( whileBegin );
}

////==========================
//// CWhileBeginNode functions
////==========================
CWhileBeginNode::~CWhileBeginNode(){

	if( whileEnd ){

		//delete this->whileEnd;
		//whileEnd = 0;
	}
}

CBaseNode* CWhileBeginNode::Execute(){

	if( Conditional( condition ) ){
		//cout<<"Looping: \n";
		return this->next;
	}
	else{
		//cout<<"End Loop:\n";
		return whileEnd->next;
	}
}

bool CWhileBeginNode::Conditional(std::string condition){

	if((condition[0] == '(') && ((condition.size() - 1) ==  (MatchParentheses(condition.substr(1)) + 1)))
	{	
		condition = condition.substr(1,condition.size() - 2);
	}

	int delPos;
	int delType = ConditionDelimiter(condition,delPos);

	string curExp = condition.substr(0,delPos);

	switch(delType)
	{
	case EQUAL:
		return EvalExp<float>(Owner, curExp) == EvalExp<float>(Owner,condition.substr(delPos+2));
	case NOT_EQUAL:
		return EvalExp<float>(Owner, curExp) != EvalExp<float>(Owner,condition.substr(delPos+2));
	case LESS_THAN:
		return EvalExp<float>(Owner, curExp) < EvalExp<float>(Owner,condition.substr(delPos+1));
	case LESS_OR_EQUAL_TO:
		return EvalExp<float>(Owner, curExp) <= EvalExp<float>(Owner,condition.substr(delPos+2));
	case GREATER_THAN:
		return EvalExp<float>(Owner, curExp) > EvalExp<float>(Owner,condition.substr(delPos+1));
	case GREATER_OR_EQUAL_TO:
		return EvalExp<float>(Owner, curExp) >= EvalExp<float>(Owner,condition.substr(delPos+2));
	case AND:
		return Conditional(curExp) && Conditional(condition.substr(delPos+2));
		//return EvalExp<int>(Owner, curExp) && EvalExp<int>(Owner,condition.substr(delPos+2));
	case OR:
		return Conditional(curExp) || Conditional(condition.substr(delPos+2));
		//return EvalExp<int>(Owner, curExp) || EvalExp<int>(Owner,condition.substr(delPos+2));
	case NOT:
		return !(Conditional(condition.substr(1)));
	default:
		return EvalExp<float>(Owner, curExp);
	}

	return false;
}


CSleepNodeNode::~CSleepNodeNode(){
	

}

CBaseNode* CSleepNodeNode::Execute(){

	timeSleeping += dt;

	if(timeSleeping >= SLEEP_TIME)
	{
		timeSleeping = 0.0f;
		return this->next;
	}	

	return this;
}
