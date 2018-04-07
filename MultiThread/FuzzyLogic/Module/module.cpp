#include <stdarg.h>
#include <iostream>
#include <cassert>

#include "Module.h"


Module::~Module()
{
  VarMap::iterator currentVar = m_Variable.begin();
  for (currentVar; currentVar != m_Variable.end(); ++currentVar)
  {
    delete currentVar->second;
  }

  std::vector<Rules*>::iterator currentRule = m_Rules.begin();
  for (currentRule; currentRule != m_Rules.end(); ++currentRule)
  {
    delete *currentRule;
  }
}


void Module::AddRule(Terms& antecedent, Terms& consequence)
{
  m_Rules.push_back(new Rules(antecedent, consequence));
}

 

Variables& Module::CreateFLV(const std::string& VarName)
{
  m_Variable[VarName] = new Variables();;

  return *m_Variable[VarName];
}

