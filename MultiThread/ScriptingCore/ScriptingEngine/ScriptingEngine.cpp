#include "ScriptingEngine.h"

CScriptingEngine::CScriptingEngine() : mEventMgr(),mNFactory(), mCompileLog( "COMPILE_LOG.txt" ){

	mScriptHandle = FindFirstFile( "Scripts/*.txt", &mScriptData );
	GoodBuild = true;
}

CScriptingEngine::~CScriptingEngine(){

	map<string, CBaseNode*>::iterator it = mScriptMap.begin();
	while( !mScriptMap.empty() ){

		delete it->second;
		it->second = 0;


		mScriptMap.erase( it );

		it = mScriptMap.begin();
	}

	mScriptMap.clear();

	VarMgr->CleanUp();
	VarMgr->Destroy();
	FuncMgr->Destroy();

	delete VarMgr;
	delete FuncMgr;
}

void CScriptingEngine::_RunEvent( string eventName ){

	mEventMgr.RunEvent( eventName );
}

void CScriptingEngine::RunScript( string scriptName ){

	if( mScriptMap.find( scriptName ) != mScriptMap.end() ){

		CBaseNode* temp = mScriptMap.find( scriptName )->second;

		do{

			if( temp != NULL )
				temp = temp->Execute();

		}while( temp != NULL );
	}
}

void CScriptingEngine::Compile(){

	char loadFile[260];

	string currentLine;
	string currentfile;
	string lastfile;
	string currentScript;
	int line;
	int conditionLine;
	int ifCount;
	int whileCount;
	int objLock;
	int scriptLock;
	int eventLock;
	Error errors;
	EEvent Event = ERR;

	size_t wsPos, delPos1, delPos2;	//whitespace and delimiter positions for parsing
	string parseTemp1, parseTemp2, parseTemp3;  //temporary holders for parsed strings

	if( mScriptHandle != INVALID_HANDLE_VALUE ){

		currentfile = mScriptData.cFileName;
		currentScript = currentfile;
		//mNFactory.BeginNewList( currentfile );
		
		while( currentfile != lastfile ){

			cout << currentfile << endl;
			
			sprintf_s( loadFile, "Scripts/%s", mScriptData.cFileName );

			mLoadFile.open( loadFile );

			if( mLoadFile.is_open())
			{

				mCompileLog.PrintScript(currentfile);

				line = 1;

				ifCount = 0;
				whileCount = 0;
				objLock = 0;
				scriptLock = 0;
				eventLock = 0;
				while( !mLoadFile.eof() )
				{
				
					//===========================================
					// this is where we start all of our parsing!
					//===========================================

					getline( mLoadFile, currentLine );

					if( (currentLine.size() != 0) && (currentLine[0] != '/' && currentLine[1] != '/') )
					{

						RemoveLeadingWhiteSpace(&currentLine);

						wsPos = currentLine.find(" ");
						parseTemp1 = currentLine.substr(0,wsPos);

						delPos2 = currentLine.find("(");
						parseTemp2 = currentLine.substr(0,delPos2);
						RemoveWhiteSpace(&parseTemp2);

						//RemoveLeadingWhiteSpace(parseTemp1);

						if(parseTemp1 == "obj")
						{
							parseTemp2 = currentLine.substr(wsPos);
							RemoveLeadingWhiteSpace(&parseTemp2);

							if(objLock > 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,NESTED_OBJ);
							}

							if(parseTemp2 == "")
							{
								GoodBuild = false;
								mCompileLog.Print(line,NO_OBJ_NAME);
							}

							//mNFactory.BeginNewList(parseTemp2);
							objLock++;
							currentScript = parseTemp2;
						}

						else if(currentLine == "endobj")
						{
							objLock--;
						}

						else if(parseTemp1 == "event")
						{
							parseTemp2 = currentLine.substr(wsPos);
							RemoveLeadingWhiteSpace(&parseTemp2);

							if(eventLock > 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,NESTED_EVENT);
							}

							if(parseTemp2 == "")
							{
								GoodBuild = false;
								mCompileLog.Print(line,NO_EVENT_NAME);
							}
							
							if(parseTemp2 == "LEFT_CLICK")
							{
								Event = LEFT_CLICK;
							}

							else if(parseTemp2 == "RIGHT_CLICK")
							{
								Event = RIGHT_CLICK;
							}

							else if(parseTemp2 == "COLLISION")
							{
								Event = COLLISION;
							}

							if(Event == ERROR)
							{
								GoodBuild = false;
								mCompileLog.Print(line,NO_SUCH_EVENT);
							}

							eventLock++;
							mNFactory.BeginNewList(parseTemp2);

						}

						else if(currentLine == "endevent")
						{
							mObjMgr.AddObjectEvent(currentScript,Event,mNFactory.GetCurrentListStart());
							eventLock--;
							Event = ERR;
							VarList.clear();
							currentScript = "";
						}
						
						else if(currentLine == "endscript")
						{
							if(objLock > 0)
							{
								mObjMgr.AddObject(currentScript, mNFactory.GetCurrentListStart());
							}
							else
							{
								mScriptMap[ currentScript ] = mNFactory.GetCurrentListStart();
							}
							scriptLock--;
							currentScript = "";
							VarList.clear();
						}

						else if(currentLine.find("script")!= string::npos)
						{
							if(currentLine == "script")
							{
								parseTemp2 = "";
								
							}

							else
							{
								parseTemp2 = currentLine.substr(wsPos);
								RemoveLeadingWhiteSpace(&parseTemp2);
							}

							

							//if(parseTemp2 == "" && objCount == 1)
							//{
							//	//mNFactory.BeginNewList(currentScript);
							//}
							if(parseTemp2 != "" && objLock == 0)
							{
								currentScript = parseTemp2;
								//mNFactory.BeginNewList(currentScript);	
							}

							else if(parseTemp2 != "" && objLock > 0)
							{
								//error
								GoodBuild = false;
								mCompileLog.Print(line,NAME_NOT_NEEDED);
							}

							if(scriptLock != 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,NESTED_SCRIPT);
							}

							if(currentScript == "")
							{
								GoodBuild = false;
								mCompileLog.Print(line,NO_SCRIPT_NAME);
							}


							mNFactory.BeginNewList(currentScript);

							scriptLock++;

						}

						else if((parseTemp1 == "var") || (parseTemp1 == "gvar"))
						{
							int scope;
							if(parseTemp1 == "var")
							{
								scope = 0;
							}

							else
							{
								scope = 1;
							}

							parseTemp2 = currentLine.substr(wsPos);
							RemoveLeadingWhiteSpace(&parseTemp2);
							
							wsPos = parseTemp2.find(" ");
							
							parseTemp3 = parseTemp2.substr(0,wsPos);
							parseTemp2 = parseTemp2.substr(wsPos+1);
							RemoveLeadingWhiteSpace(&parseTemp2);
							delPos1 = parseTemp2.find("=");
							if(delPos1 != string::npos)
							{
								parseTemp1 = parseTemp2.substr(0,delPos1);
								parseTemp2 = parseTemp2.substr(delPos1+1);
								RemoveWhiteSpace(&parseTemp1);
								RemoveWhiteSpace(&parseTemp2);

								if(!CheckExp(parseTemp2, errors))
								{
									GoodBuild = false;
									mCompileLog.Print(line, errors);
									
								}
								VarList.push_back(parseTemp1);
								mNFactory.AddVariableNode(parseTemp3,parseTemp1,parseTemp2,scope,line);	
							}

							else
							{
								delPos2 = parseTemp2.find(";");
								parseTemp1 = parseTemp2.substr(0,delPos2);
								RemoveWhiteSpace(&parseTemp1);
								VarList.push_back(parseTemp1);

								mNFactory.AddVariableNode(parseTemp3,parseTemp1,"",scope ,line);	
							}
						}
						
						else if((parseTemp2 == "if") || (parseTemp2 == "while"))
						{
							parseTemp1 = currentLine.substr(delPos2+1);
							delPos1 = parseTemp1.find_last_of(")");

							parseTemp1 = parseTemp1.substr(0,delPos1);
							RemoveWhiteSpace(&parseTemp1);
							
							if(!CheckExp(parseTemp1, errors))
							{
									GoodBuild = false;
									mCompileLog.Print(line, errors);
									
							}
							if(parseTemp2 == "if")
							{
								ifCount++;
								mNFactory.AddConditionalNode(parseTemp1,IF, line);
							}

							else
							{
								whileCount++;
								mNFactory.AddConditionalNode(parseTemp1,WHILE, line);
							}
							
						}
						//Create Assignment node
						else if(currentLine.find("=") != string::npos) 
						{
							delPos1 = currentLine.find("=");
							parseTemp1 = currentLine.substr(0,delPos1);
							parseTemp2 = currentLine.substr(delPos1+1);

							RemoveWhiteSpace(&parseTemp1);
							RemoveWhiteSpace(&parseTemp2);
							
							if(!CheckExp(parseTemp2, errors))
							{
								GoodBuild = false;
								mCompileLog.Print(line, errors);
									
							}

							mNFactory.AddAssignmentNode(parseTemp1,parseTemp2, line);
							
						}

						//Parse functions
						else if( currentLine.find( '(' ) != string::npos )
						{

							delPos1 = currentLine.find( '(' );
							delPos2 = currentLine.find( ')' );

							string fName = currentLine.substr(0, delPos1);
							string args  = currentLine.substr(delPos1 + 1, (delPos2 - delPos1) - 1 );

							RemoveWhiteSpace(&fName);
							RemoveWhiteSpace(&args);

							if(!CheckExp(args, errors))
							{
								GoodBuild = false;
								mCompileLog.Print(line, errors);
								
							}

							vector<string> vargs;
		
							//getting arguments into a vector of strings
							if( args.size() > 0 ){

								string temp;
								delPos1 = args.find(',');

								if( delPos1 != string::npos ){

									while( delPos1 != string::npos ){

										temp = args.substr( 0, delPos1 );

										vargs.push_back( temp );

										args = args.substr( delPos1 + 1, -1 );

										delPos1 = args.find( ',' );

										if( delPos1 == string::npos )
											vargs.push_back( args );

									}
								}
								else{

									vargs.push_back( args );
								}
							}

							mNFactory.AddFunctionNode( FuncMgr->GetFunction( fName ), vargs, line );
						}
						

						else if((currentLine.find("endw") != string::npos) ||(currentLine.find("endwhile") != string::npos))
						{
							whileCount--;
							if(whileCount < 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,ILLEGAL_END_WHILE);
							}
							else
							{
							mNFactory.AddWhileEnd(line);
							}
						}

						else if(currentLine.find("endif") != string::npos)
						{
							ifCount--;
							if(ifCount < 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,ILLEGAL_END_IF);
							}
							else
							{
								
								mNFactory.AddIfEnd(line);
							}
						}

						else if(currentLine == "else")
						{
							if(ifCount <= 0)
							{
								GoodBuild = false;
								mCompileLog.Print(line,ILLEGAL_ELSE);
							}
							else
							{
								mNFactory.AddElseNode(line);
							}
						}

					}//end if( currentLine.size() != 0 || currentLine[0] != '/' && currentLine[1] != '/' )
					
					line++;

				}//end while( !mLoadFile.eof() )

				if(ifCount != 0)
				{
					GoodBuild = false;
					mCompileLog.Print(line,NO_END_IF);
				}

				if(whileCount != 0)
				{
					GoodBuild = false;
					mCompileLog.Print(line,NO_END_WHILE);
				}

				if(objLock != 0)
				{
					GoodBuild = false;
					mCompileLog.Print(line,NO_END_OBJ);
				}

				if(scriptLock != 0)
				{
					GoodBuild = false;
					mCompileLog.Print(line,NO_END_SCRIPT);
				}

				if(eventLock != 0)
				{
					GoodBuild = false;
					mCompileLog.Print(line,NO_END_EVENT);
				}

				//VarList.clear();
				if(GoodBuild)
				{
					mCompileLog.Print(0, NONE);
				}
				lastfile = currentfile;

				//mScriptMap[ currentfile ] = mNFactory.GetCurrentListStart();

				FindNextFile( mScriptHandle, &mScriptData );

				currentfile = mScriptData.cFileName;
				currentScript = currentfile;

				//mNFactory.BeginNewList( currentfile );

				mLoadFile.close();
				mLoadFile.clear();


			}//end if( mLoadFile.is_open())
			else{
				
				//File did not open
			}
			
		}//end while( compiling )
		

	}//end if( mScriptHandle != INVALID_HANDLE_VALUE )
	else{

		//It didn't find any data
	}
}

void CScriptingEngine::ReCompile(){

	map<string, CBaseNode*>::iterator it = mScriptMap.begin();
	while( !mScriptMap.empty() ){

		delete it->second;
		it->second = 0;

		mScriptMap.erase( it );

		it = mScriptMap.begin();
	}

	GoodBuild = true;

	mScriptMap.clear();

	VarMgr->CleanUp();

	Compile();
}

bool CScriptingEngine::CheckExp(string exp, Error& errorType)
{
	if(!CountParens(exp))
	{
		errorType = PAREN_MISMATCH;
		return false;
	}
	
	if(exp.find_first_not_of("_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!%&|*/<>()-+=,.;\"") != string::npos)
	{
		errorType = ILLEGAL_CHAR;
		return false;
	}
	string temp = exp;
	string checkStr;
	int endStr;

	int i = 0;
	while(i < exp.size())
	{
		endStr = exp.substr(i).find_first_of("!%&|*/<>()-+=,;") + 1;
		
		//char bob = exp[endStr];

		if(endStr <= i)
		{
			i++;
		}
		else
		{
			if((exp[i] >= 65) && !CheckVars(exp.substr(i, endStr-1)))
			{
				errorType = VAR_NOT_FOUND;
				return false;
			}

			i += endStr;
		}
	}


	errorType = NONE;
	return true;
}

bool CScriptingEngine::CountParens(string exp)
{
	int count = 0;
	for(int i = 0; i < exp.size(); i++)
	{
		if(exp[i] == '(')
		{
			count++;
		}

		else if(exp[i] == ')')
		{
			count--;
		}

		if(count < 0)
		{
			return false;
		}
	}

	if(count != 0)
	{
		return false;
	}

	return true;
}

bool CScriptingEngine::CheckVars(string str)
{
	for(int i = 0; i < VarList.size(); i++)
	{
		if(VarList[i] == str)
		{
			return true;
		}
	}

	if(FuncMgr->FindFunction(str))
	{
		return true;
	}

	return false;
}

void CScriptingEngine::Debug()
{
	
}

void CScriptingEngine::RemoveLeadingWhiteSpace(string* str)
{
	if( str->size() != 0 ){

		while((str->at(0) == ' ') || (str->at(0) == '\t'))
		{
			str->erase(0,1);
		}
	}
}

void CScriptingEngine::RemoveWhiteSpace(string* str)
{
	for(unsigned int i = 0; i < str->size(); i++)
	{
		if((str->at(i) == ' ') || (str->at(i) == '\t'))
		{
			str->erase(i,1);
			i--;
		}
	}
}

void CScriptingEngine::UpdateAllScripts(float dt){

	map<string, CBaseNode*>::iterator it = mScriptMap.begin();

	for( ;it != mScriptMap.end();++it ){

		CBaseNode* temp = it->second;

		do{
			
			temp = temp->Execute();

		}while( temp != NULL );
	}
}

bool CScriptingEngine::IsType( string type ){

	return mObjMgr.IsType( type );
}

bool CScriptingEngine::HasEvent( string type, EEvent e ){

	return mObjMgr.HasEvent( type, e );
}

void CScriptingEngine::RunObjectScript( string type ){

	mObjMgr.RunObjectScript( type );
}

void CScriptingEngine::RunObjectEvent( string type, EEvent e ){

	mObjMgr.RunObjectEvent( type, e );
}

CBaseNode* CScriptingEngine::GetObjectScript( string obj ){

	return mObjMgr.GetObjectScript( obj );
}