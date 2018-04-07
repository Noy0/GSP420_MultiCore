#include "Module\module.h"

#include <vector>
#include <string>
#include <map>
#include <iosfwd>
#include <fstream>
#include <iostream>
using namespace std;

Module ts;
double LastScore;
double DistToTarget;

void Initialize_Fuzzy();
double GetDesirability(double);


void main()
{


}

void Initialize_Fuzzy()
{
	
	Variables& joe = ts.CreateFLV("joe");
	Variables& Desire = ts.CreateFLV("Desire");
	
	ProxySet& nerd = joe.AddLeftShoulderSet("nerd",0,5,10);
	ProxySet& normal = joe.AddTriangularSet("normal",7,10,13);
	ProxySet& cool = joe.AddRightShoulderSet("cool",12,16,20);

	ProxySet& UnDesirable = Desire.AddLeftShoulderSet("UnDesirable",0,25,50);
	ProxySet& Desirable = Desire.AddTriangularSet("Desirable",25,50,75);
	ProxySet& Very_Desirable = Desire.AddRightShoulderSet("Very_Desirable",50,75,100);
	
	ts.AddRule(nerd,UnDesirable);
	ts.AddRule(normal,Desirable);
	ts.AddRule(cool, Very_Desirable);
	gModule->AddRule(cool,Very_Desirable);

}
double GetDesirability(double DistToTarget)
{
    ts.Fuzzify("DistanceToTarget", DistToTarget);
    LastScore = ts.DeFuzzify("Desire");

  return LastScore;
}
