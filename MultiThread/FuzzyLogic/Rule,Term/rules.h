#pragma once

#include <vector>
#include "..\Sets\Sets.h"
#include "Terms.h"
#include "..\..\Utils\utils.h"


class Rules
{ 
private:

  Terms*  m_Ant;

  Terms*        m_Con;

  Rules(const Rules&);
  Rules& operator=(const Rules&);


public:

  Rules(const Terms& ant,const Terms& con):m_Ant(ant.Clone()), m_Con(con.Clone()){}

  ~Rules(){delete m_Ant; delete m_Con;}

  void SetToZero(){m_Con->ClearDOM();}

  void Calculate()
  {
    m_Con->ORwithDOM(m_Ant->GetDOM());
  }
};

