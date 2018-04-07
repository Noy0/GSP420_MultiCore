#ifndef CMOVEMENT_ALGORITHMS
#define CMOVEMENT_ALGORITHMS

#include "SVector3D.h"
#include <vector>
#include <math.h>
#include "..\Utils\utils.h"
#include "Transformations.h"
#include <d3dx9.h>

#include ".\DD_AIData\DDSTeeringObstacles.h"
#include ".\DD_AIData\DDSteering.h"


using std::pair;
using std::vector;

//------------------------------------------------------------------------
//
//	Name: CMovement_Algorithms.h
//
//  Desc: AI decisions on how,why, and where to move.
//
//  Author: Derek Hofferber
//
//------------------------------------------------------------------------

// all variables that Movement algorithms need access to
struct SteeringData
{
	int ID; //id of the entity, should be the same as the index in the vector.
	
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
	//bool m_bFlockingTagged;                                               //determins if the entity has been tagged for flocking.

	//if true then an obstacle is near and must be avoided. might not be needed.
	bool m_bObstTagged;                                                   //determins if the entity has been tagged for obstacle avoidance.  

	//use these 2 ID's if you are only evading/persuing one single entity.
	//these could also be used to represent types of entities if you want to evade or pursuit every entity of a certain type.
	int m_iEvadeID; //id of the entity that the current entity will be evading from.
	int m_iPursuitID; // id of the entity that the current entity will be persuing.

	//use bools if more than one entity needs to be affected.
	//chases other entities
	bool m_bEntityisPursuing;                                            //if true then others will know to evade from this entity if their evade is on.
	
	//if this entity is Pursuing then this type will determine what other entity types it needs to pursue.
	int m_iPursuingType; //this type can represent chasing entities of this class type.
	
	//runs away from other entites
	bool m_bEntityisEvading;                                              //if true the others that will know to pursuit this entity if their pursuit is on.

	//steering flags determine which movement algorithms are being used.
	int m_iSteeringFlags; //this is a bitflag.

	int m_iMinDetectionBoxLength; //used in obstacle avoidance
	double m_dMaxSpeed;
	double m_dSpeed;
	double m_dRadius;
	double m_dScale;
	double m_dMaxForce;
	//double m_dMass; //not needed if given SHeading
	//double m_dTimeElapsed; //not needed if given SHeading
	SVector3D SPosition;
	SVector3D SVelocity;
	//can get heading from velocity;
	SVector3D SHeading;

	//steering weights. determins how important each movement algorithm is.
};

class CMovement_Algorithms
{
private:
//--------------Movement Weights-----------
//multipliers. These can be adjusted to effect strength of the  
//appropriate behavior. Useful to get flocking the way you require
//for example.
	double m_dWeightSeparation;
	double m_dWeightCohesion;
	double m_dWeightAlignment;
	double m_dWeightWander;	
	double m_dWeightObstacleAvoidance; 
	double m_dWeightSeek;				
	double m_dWeightFlee;
	double m_dWeightArrive;		
	double m_dWeightPursuit;	
	double m_dWeightInterpose;	
	double m_dWeightHide;
	double m_dWeightEvade;
//-------------------------------------------------------
private:
	
//--------------------------- Constants ----------------------------------
const double m_dViewDistance;
//the radius of the constraining circle for the wander behavior
const double WanderRad;
//distance the wander circle is projected in front of the agent
const double WanderDist;
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec;

private:
	enum behavior_type
	{
		none               = 0x00000,
		seek               = 0x00002,   //will set up
		flee               = 0x00004,   // will set up
		arrive             = 0x00008, //using
		wander             = 0x00010, //using
		cohesion           = 0x00020,
		separation         = 0x00040,
		allignment         = 0x00080,
		obstacle_avoidance = 0x00100,
		pursuit            = 0x00800,
		evade              = 0x01000,
		//interpose          = 0x02000,
		hide               = 0x04000, //using
		flock              = 0x08000,
	};


  //Arrive makes use of these to determine how quickly a vehicle
  //should decelerate to its target
  enum Deceleration{slow = 3, normal = 2, fast = 1};
private:
//----------------Data---------------
	SteeringData SCurEntity; //current entity

//----------------Vectors------------
	vector<SteeringObstacles> m_vObstData; //vector of all entities
	vector<SteeringData> m_vEntityData; //vector of all obstacles
	vector<SteeringObstacles> m_vAvoidObst; //vector of all obstacles that have been tagged for avoidance.
	vector<SteeringData> m_vPursuers; //vector of all entities that will be pursuing.
	vector<SteeringData> m_vFlockers; //vector of all entites that will be flocking.
	vector<pair<int,SVector3D>> m_vFinalSteering; //vector of all the final steering forces of every entity.
//----------------Structs-------------
	SVector3D			SSteeringForce; //m_dRotForce
	SVector3D			STarget;		//location of closest obj nearby to seek/persue.
	SVector3D			SPursuer;		//location of the object to flee from. entities are fleeing from this
	SVector3D			SWanderTarget; 	  //the current position on the wander circle the agent is
//----------------Doubles----------------
	double			    m_dWanderJitter;
	double			    m_dWanderRadius;
	double			    m_dWanderDistance;
	double				m_dDBoxLength;
//----------------Ints-------------------
	//current behavior type
	int					m_iFlags;
//----------------Enums------------------
	//slowing down in arrive
	Deceleration m_eDeceleration;
//this function tests if a specific bit of m_iFlags is set
	bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}

public:
CMovement_Algorithms():
						m_iFlags(0),
						m_dDBoxLength(20),
						m_eDeceleration(normal),
						m_dWanderJitter(WanderJitterPerSec),
						m_dWanderRadius(WanderRad),
						m_dWanderDistance(WanderDist),
						m_dWeightSeparation(2.0),
						m_dWeightCohesion(1.0),
						m_dWeightAlignment(1.0),
						m_dWeightWander(1.0),
						m_dWeightObstacleAvoidance(2.0),
						m_dWeightSeek(1.0),
						m_dWeightFlee(1.0),
						m_dWeightArrive(1.0),
						m_dWeightPursuit(1.0),
						m_dWeightInterpose(1.0),
						m_dWeightHide(1.0),
						m_dWeightEvade(2.0),
						m_dViewDistance(25.0),
						WanderRad(1.2),
						WanderDist(2.0),
						WanderJitterPerSec(80.0)
{

	//SWanderTarget = SVector3D((RandFloat() * CParams::WindowWidth),(RandFloat() * CParams::WindowHeight));
	//m_vSide = m_vHeading.Perp();
	double theta = RandFloat() * TwoPi;

	//create a vector to a target position on the wander circle
	SWanderTarget = SVector3D(m_dWanderRadius * cos(theta),m_dWanderRadius * sin(theta));
}

~CMovement_Algorithms()
{

}

//----------------------------accessor functions
//---------------------BitFlag Accessors-----------------------//
//---Turn on BitFlag Accessors---//
void FleeOn(){m_iFlags |= flee;}
void SeekOn(){m_iFlags |= seek;}
void ArriveOn(){m_iFlags |= arrive;}
void WanderOn(){m_iFlags |= wander;}
void PursuitOn(){m_iFlags |= pursuit;}
void EvadeOn(){m_iFlags |= evade;}  
//void PursuitOn(Entity_Class_Type1* v){m_iFlags |= pursuit; m_pTarget = v;}
//void EvadeOn(Entity_Class_Type1* v){m_iFlags |= evade; m_pPersuer = v;}
void CohesionOn(){m_iFlags |= cohesion;}
void SeparationOn(){m_iFlags |= separation;}
void AlignmentOn(){m_iFlags |= allignment;}
void ObstacleAvoidanceOn(){m_iFlags |= obstacle_avoidance;}
//void InterposeOn(Vehicle* v1, Vehicle* v2){m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2;}
void HideOn(){m_iFlags |= hide;}
//void HideOn(Entity_Class_Type1* v){m_iFlags |= hide; m_pPersuer = v;}
void FlockingOn(){CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn();}
//---Turn off BitFlag Accessors---//
void FleeOff()  {if(On(flee))   m_iFlags ^=flee;}
void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
void WanderOff(){if(On(wander)) m_iFlags ^=wander;}
void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
void EvadeOff(){if(On(evade)) m_iFlags ^=evade;}
void CohesionOff(){if(On(cohesion)) m_iFlags ^=cohesion;}
void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
void AlignmentOff(){if(On(allignment)) m_iFlags ^=allignment;}
void ObstacleAvoidanceOff(){if(On(obstacle_avoidance)) m_iFlags ^=obstacle_avoidance;}
//void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}
void HideOff(){if(On(hide)) m_iFlags ^=hide;}
void FlockingOff(){CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff();}
//---Check on/off BitFlag Accessors---//
bool isFleeOn(){return On(flee);}
bool isSeekOn(){return On(seek);}
bool isArriveOn(){return On(arrive);}
bool isWanderOn(){return On(wander);}
bool isPursuitOn(){return On(pursuit);}
bool isEvadeOn(){return On(evade);}
bool isCohesionOn(){return On(cohesion);}
bool isSeparationOn(){return On(separation);}
bool isAlignmentOn(){return On(allignment);}
bool isObstacleAvoidanceOn(){return On(obstacle_avoidance);}
//bool isInterposeOn(){return On(interpose);}
bool isHideOn(){return On(hide);}

// set weights for movement algorithms
void SetWeightSeparation(double weight){m_dWeightSeparation = weight;}
void SetWeightCohesion(double weight){m_dWeightCohesion = weight;}
void SetWeightAlignment(double weight){m_dWeightAlignment = weight;}
void SetWeightWander(double weight){m_dWeightWander = weight;}
void SetWeightObstacleAvoidance(double weight){m_dWeightObstacleAvoidance = weight;}
void SetWeightSeek(double weight){m_dWeightSeek = weight;}
void SetWeightFlee(double weight){m_dWeightFlee = weight;}
void SetWeightArrive(double weight){m_dWeightArrive = weight;}
void SetWeightPursuit(double weight){m_dWeightPursuit = weight;}
void SetWeightInterpose(double weight){m_dWeightInterpose = weight;}
void SetWeightHide(double weight){m_dWeightHide = weight;}
void SetWeightEvade(double weight){m_dWeightEvade = weight;}

//----------------------Begin Functions------------------------//

vector<pair<int,SVector3D>> Update(vector<SteeringData> Entity, vector<SteeringObstacles> Obst);
D3DXVECTOR3 AriveTemp(D3DXVECTOR3 CurPos, D3DXVECTOR3 TargetPos);
private:

void SetAlgorithms(int Flags);

void SetEntitiesPursuing();

void SetEntitiesFlocking();

SVector3D Calculate(SteeringData Entity);

SteeringData GetclosestPursuer();

SteeringData GetclosestEntityTypeBeingPursued();

bool AccumulateForce(SVector3D &RunningTot, SVector3D ForceToAdd);

SVector3D Seek(SVector3D TargetPos);

SVector3D Flee(SVector3D TargetPos);

SVector3D Arrive(SVector3D TargetPos, Deceleration deceleration);

SVector3D Pursuit(SteeringData evader);

SVector3D Evade(SteeringData pursuer);

SVector3D Wander();

SVector3D ObstacleAvoidance();

SVector3D Separation();

SVector3D Alignment();

SVector3D Cohesion();

SVector3D Hide(SteeringData hunter);

SVector3D GetHidingPosition(const SVector3D& posOb, const double radiusOb, const SVector3D& posHunter);

};



#endif