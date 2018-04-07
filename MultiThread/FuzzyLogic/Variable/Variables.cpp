#include "Variables.h"
#include "..\Sets\Set_triangle.h"
#include "..\Sets\Set_LeftShoulder.h"
#include "..\Sets\Set_RightShoulder.h"
#include "..\Sets\Set_Singleton.h"
#include "..\Sets\ProxySet.h"
#include "..\..\Utils\utils.h"
#include <cassert>
#include <iostream>
#include <algorithm>


Variables::~Variables()
{
  MemberSets::iterator it;
  for (it = m_MemberSets.begin(); it != m_MemberSets.end(); ++it)
  {
    delete it->second;
  }
}

void Variables::Fuzzify(double val)
{    
  //make sure the value is within the bounds of this variable
  /*assert ( (val >= m_dMinRange) && (val <= m_dMaxRange) && 
           "<FuzzyVariable::Fuzzify>: value out of range");*/

  //for each set in the flv calculate the DOM for the given value
  MemberSets::const_iterator curSet;
  for (curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    curSet->second->SetDOM(curSet->second->CalculateDOM(val));
  }
}


double Variables::DeFuzzifyMaxAv()const
{
  double bottom = 0.0;
  double top    = 0.0;

  MemberSets::const_iterator curSet;
  for (curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    bottom += curSet->second->GetDOM();

    top += curSet->second->GetRepresentativeVal() * curSet->second->GetDOM();
  }

  //make sure bottom is not equal to zero
  if (isEqual(0, bottom)) return 0.0;

  return top / bottom;   
}


ProxySet Variables::AddTriangularSet(string name, double minBound,double peak, double maxBound)
{
  m_MemberSets[name] = new Set_Triangle(peak,
                                             peak-minBound,
                                             maxBound-peak);
  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return ProxySet(*m_MemberSets[name]);
}

ProxySet Variables::AddLeftShoulderSet(std::string name,
                                        double       minBound,
                                        double       peak,
                                        double       maxBound)
{
  m_MemberSets[name] = new Set_LeftShoulder(peak, peak-minBound, maxBound-peak);

  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return ProxySet(*m_MemberSets[name]);
}


ProxySet Variables::AddRightShoulderSet(std::string name,
                                         double       minBound,
                                         double       peak,
                                         double       maxBound)
{
  m_MemberSets[name] = new Set_RightShoulder(peak, peak-minBound, maxBound-peak);

  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return ProxySet(*m_MemberSets[name]);
}

ProxySet Variables::AddSingletonSet(std::string name,
                                    double       minBound,
                                    double       peak,
                                    double       maxBound)
{
  m_MemberSets[name] = new Set_Singleton(peak,
                                              peak-minBound,
                                              maxBound-peak);

  AdjustRangeToFit(minBound, maxBound);

  return ProxySet(*m_MemberSets[name]);
}


void Variables::AdjustRangeToFit(double minBound, double maxBound)
{
  if (minBound < m_dMinRange) m_dMinRange = minBound;
  if (maxBound > m_dMaxRange) m_dMaxRange = maxBound;
}

