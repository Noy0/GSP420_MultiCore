#pragma once

#include <map>
#include <iosfwd>
#include <string>

class Sets;
class ProxySet;
class Module;

using namespace std;

class Variables
{
private:
  
  typedef std::map<std::string, Sets*>  MemberSets;
    
private:
  
  Variables(const Variables&);
  Variables& operator=(const Variables&);

private:
 
  MemberSets   m_MemberSets;

  double        m_dMinRange;
  double        m_dMaxRange;
  

  void AdjustRangeToFit(double min, double max);

  ~Variables();

  friend class Module;


public:

  Variables():m_dMinRange(0.0),m_dMaxRange(0.0){}

  std::string str;
 // ProxySet addString(string thing);

  ProxySet AddLeftShoulderSet(string name, double minBound, double peak, double maxBound);

  ProxySet  AddRightShoulderSet(string name, double minBound, double peak, double maxBound);

  ProxySet  AddTriangularSet(string name, double minBound, double peak, double maxBound);

  ProxySet  AddSingletonSet(string name, double minBound, double peak, double maxBound);
    
  void        Fuzzify(double val);

  double       DeFuzzifyMaxAv()const;

};



