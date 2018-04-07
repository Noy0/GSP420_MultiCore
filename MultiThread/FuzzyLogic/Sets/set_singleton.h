#pragma once

#include "Sets.h"
#include "..\..\Utils\utils.h"



class Set_Singleton : public Sets
{
private:

    //the values that define the shape of this FLV
  double   m_dMidPoint;
  double   m_dLeftOffset;
  double   m_dRightOffset;

public:
  
  Set_Singleton(double       mid,
                     double       lft,
                     double       rgt):Sets(mid),
                                      m_dMidPoint(mid),
                                      m_dLeftOffset(lft),
                                      m_dRightOffset(rgt)
  {}

  //this method calculates the degree of membership for a particular value
  double     CalculateDOM(double val)const; 
};

double Set_Singleton::CalculateDOM(double val)const
{
  if ( (val >= m_dMidPoint-m_dLeftOffset) &&
       (val <= m_dMidPoint+m_dRightOffset) )
  {
    return 1.0;
  }


  else
  {
    return 0.0;
  }
}