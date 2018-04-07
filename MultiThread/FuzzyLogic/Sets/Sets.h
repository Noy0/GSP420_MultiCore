#pragma once

#include <string>
#include <cassert>


class Sets
{
protected:
  
  double        m_dDOM;
  double        m_RepValue;

public:

  Sets(double RepVal):m_dDOM(0.0), m_RepValue(RepVal){}

  virtual double      CalculateDOM(double val)const = 0;

  void               ORwithDOM(double val){if (val > m_dDOM) m_dDOM = val;}

  double             GetRepresentativeVal()const{return m_RepValue;}
  
  void               ClearDOM(){m_dDOM = 0.0;}  
  double             GetDOM()const{return m_dDOM;}
  void               SetDOM(double val){m_dDOM = val;}
};

