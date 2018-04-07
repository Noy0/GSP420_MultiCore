#pragma once

#include "..\Rule,Term\Terms.h"
#include "Sets.h"

class ProxySet : public Terms
{
private:

  Sets& m_Set;

public:

  ProxySet(Sets& fs):m_Set(fs){}

  Terms* Clone()const{return new ProxySet(*this);}
  double     GetDOM()const {return m_Set.GetDOM();}
  void       ClearDOM(){m_Set.ClearDOM();}
  void       ORwithDOM(double val){m_Set.ORwithDOM(val);}
};
extern ProxySet* gSet;




