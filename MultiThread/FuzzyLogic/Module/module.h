#pragma once

#include <vector>
#include <string>
#include <map>
#include <iosfwd>

#include "..\Rule,Term\Rules.h"
#include "..\Sets\Sets.h"
#include "..\Sets\ProxySet.h"
#include "..\Rule,Term\Terms.h"
#include "..\Variable\Variables.h"


class Module 
{
private:

	typedef std::map<std::string, Variables*> VarMap;
	  

private:

	VarMap      m_Variable;

	std::vector<Rules*>   m_Rules;
 
	inline void SetToZero();


public:

	~Module();

	Variables&  CreateFLV(const std::string& VarName);
	  
	void     AddRule(Terms& antecedent, Terms& consequence);

	inline void     Fuzzify(const std::string& NameOfFLV, double val);

	inline double   DeFuzzify(const std::string& key);


};



void Module::Fuzzify(const std::string& NameOfFLV, double val)
{
  
  assert ( (m_Variable.find(NameOfFLV) != m_Variable.end()) &&
          "<FuzzyModule::Fuzzify>:key not found");

  m_Variable[NameOfFLV]->Fuzzify(val);
}


double Module::DeFuzzify(const std::string& NameOfFLV)
{
  assert ( (m_Variable.find(NameOfFLV) != m_Variable.end()) &&
          "<FuzzyModule::DeFuzzifyMaxAv>:key not found");

  SetToZero();

  std::vector<Rules*>::iterator currentRule = m_Rules.begin();
  for (currentRule; currentRule != m_Rules.end(); ++currentRule)
  {
    (*currentRule)->Calculate();
  }

 
    return m_Variable[NameOfFLV]->DeFuzzifyMaxAv();

}


void Module::SetToZero()
{
  std::vector<Rules*>::iterator currentRule = m_Rules.begin();
  for (currentRule; currentRule != m_Rules.end(); ++currentRule)
  {
    (*currentRule)->SetToZero();
  }
}

