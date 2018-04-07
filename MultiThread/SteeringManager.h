#ifndef _STEERINGMANAGER_H
#define _STEERINGMANAGER_H

#include "ResourceManager.h"
#include ".\Movement\CMovement_Algorithms.h"

//read DDSteering.h for more detailed information about each variable.

struct SteeringConstants
{	
	//This int should represent different types of entities. 
	//For example: large marbles should have the same type, 
	//where small marbles should all have their own same type.
	//Example:
	//Small_Marbles = 0;
	//Medium_Marbles = 1;
	//Large_Marbles = 2; 
	//If you wanted the pursuer to chase the large marbles you would then set
	//m_iPursuingType = to 2;
	int m_iEntityType; 

	//if true then flocking is turned on
	//bool m_bFlockingTagged; //determins if the entity has been tagged for flocking.

	//if true then an obstacle is near and must be avoided. might not be needed.
	//bool m_bObstTagged; //determins if the entity has been tagged for obstacle avoidance.  

	//use these 2 ID's if you are only evading/persuing one single entity.
	//these could also be used to represent types of entities if you want to evade or pursuit every entity of a certain type.

	//use bools if more than one entity needs to be affected.
	//chases other entities
	//bool m_bEntityisPursuing; //if true then others will know to evade from this entity if their evade is on.
	
	//if this entity is Pursuing then this type will determine what other entity types it needs to pursue.
	int m_iPursuingType; //this type can represent chasing entities of this class type.
	
	//runs away from other entites
	//bool m_bEntityisEvading; //if true the others that will know to pursuit this entity if their pursuit is on.

	//steering flags determine which movement algorithms are being used.
	int m_iSteeringFlags; //this is a bitflag.

	int m_iMinDetectionBoxLength; //used in obstacle avoidance
	double m_dMaxSpeed;
	double m_dRadius;
	double m_dScale;
	double m_dMaxForce;
	//double m_dMass; //not needed if given SHeading
	//double m_dTimeElapsed; //not needed if given SHeading

	//steering weights. determins how important each movement algorithm is.
	//double m_dWeightSeparation;
	//double m_dWeightCohesion;
	//double m_dWeightAlignment;
	//double m_dWeightWander;	
	//double m_dWeightObstacleAvoidance; 
	//double m_dWeightSeek;				
	//double m_dWeightFlee;
	//double m_dWeightArrive;		
	//double m_dWeightPursuit;	
	////double m_dWeightInterpose;	
	//double m_dWeightHide;
	//double m_dWeightEvade;
	//int Flags;

	//double MaxForce;
	//double MaxSpeed;
	SteeringConstants()
	{
	}
};

class SteeringConstantsMgr : public ResourceManager<SteeringConstants*>
{
public:
	static void Initialize();
	void AddItem(int key, SteeringConstants* item);
private:
	SteeringConstantsMgr(){}
};

extern SteeringConstantsMgr* gSCMgr;


#endif


