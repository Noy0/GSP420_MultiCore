#include ".\Movement\CMovement_Algorithms.h"



template <class T, class conT>
void TagVehiclesWithinViewRange(const T& entity, conT& ContainerOfEntities, double radius)
{
  //iterate through all entities checking for range
  for (typename conT::iterator curEntity = ContainerOfEntities.begin();
       curEntity != ContainerOfEntities.end();
       ++curEntity)
  {
    
    SVector3D to = (*curEntity).SPosition - entity.SPosition;

    //the bounding radius of the other is taken into account by adding it 
    //to the range
    double range = radius + (*curEntity).m_dRadius;

    //if entity within range, tag for further consideration. (working in
    //distance-squared space to avoid sqrts)
    if ((to.LengthSq() < range*range))
    {
		//old used to tag
    }
	else
		ContainerOfEntities.erase(curEntity);
    
  }//next entity
}

template <class T, class conT>
void TagObstaclesWithinViewRange(const T& entity, conT& ContainerOfEntities, double radius)
{
  //iterate through all entities checking for range
  for (typename conT::iterator curEntity = ContainerOfEntities.begin();
       curEntity != ContainerOfEntities.end();
       ++curEntity)
  {
    //first clear any current tag
    //(*curEntity)->m_dTagged = false; //might work
	//(*curEntity)->m_dTagged(false);
    
    SVector3D to = (*curEntity).SPosition - entity.SPosition;

    //the bounding radius of the other is taken into account by adding it 
    //to the range
    double range = radius + (*curEntity).m_dRadius;

    //if entity within range, tag for further consideration. (working in
    //distance-squared space to avoid sqrts)
    if ((to.LengthSq() < range*range))
    {
      //(*curEntity)->m_dTagged = true;
	  //(*curEntity)->m_dTagged(true);
    }
	else
		ContainerOfEntities.erase(curEntity);
    
  }//next entity
}
















vector<pair<int,SVector3D>> CMovement_Algorithms::Update(vector<SteeringData> Entity, vector<SteeringObstacles> Obst)
{
	m_vEntityData.clear(); //empty previous entity vector
	m_vObstData.clear();//empty previous obstacle vector
	m_vPursuers.clear(); //empty previous entity vector of all pursuers
	m_vEntityData = Entity; //set new vector data
	m_vObstData = Obst; //set new vector data
	SetEntitiesPursuing(); //fill vector of entities that will pursue
	//reset the steering force
	SSteeringForce.Zero();

	m_vFinalSteering.resize(Entity.size());
	//this loop is determining the current entity that is being checked against all other entities
	//for(unsigned int i = 0; i < m_vEntityData.size(); i++)
	for(unsigned int i = 0; i < Entity.size(); i++)
	{
		m_iFlags = 0;
		 // set flags to determine what the entity is doing.
		SetAlgorithms(Entity[i].m_iSteeringFlags);

		m_dDBoxLength = Entity[i].m_iMinDetectionBoxLength;
		m_vAvoidObst.clear(); //empty previous entity vector of all obstacles.
		m_vFlockers.clear(); //empty previous entity vector of all nearby entities flocking.
		m_vFlockers = Entity; //fill vector with all entities.
		m_vAvoidObst = m_vObstData; //reset vector with entire list of data so it can be modified.
		//Use the standard tagging system
		//tag neighbors if any of the following 3 group behaviors are switched on.
		if (On(separation) || On(allignment) || On(cohesion))
		{
			//tag entities in the vector that are flocking and near by.
			TagVehiclesWithinViewRange(Entity[i], m_vFlockers, m_dViewDistance); 
		}
		SetEntitiesFlocking(); //remove entities from the vector that are not flocking.
		
		SSteeringForce = Calculate(Entity[i]);

		m_vFinalSteering[i].first = Entity[i].ID;
		m_vFinalSteering[i].second = SSteeringForce;
	}

    return m_vFinalSteering;
}
D3DXVECTOR3 CMovement_Algorithms::AriveTemp(D3DXVECTOR3 CurPos, D3DXVECTOR3 TargetPos)
{
	D3DXVECTOR3 ToTarget = TargetPos - CurPos;

	SVector3D temp;
	temp.x = ToTarget.x;
	temp.y = ToTarget.z;

  //calculate the distance to the target
  double dist = temp.Length();

  //if (dist > 0)
  //{
    //because Deceleration is enumerated as an int, this value is required
    //to provide fine tweaking of the deceleration..
    const double DecelerationTweaker = 0.3;

    //calculate the speed required to reach the target given the desired
    //deceleration
    double speed =  dist / ((double)normal * DecelerationTweaker);     

    //make sure the velocity does not exceed the max
    speed = min(speed, 10);

    //from here proceed just like Seek except we don't need to normalize 
    //the ToTarget vector because we have already gone to the trouble
    //of calculating its length: dist. 
    SVector3D DesiredVelocity =  temp * speed / dist;

	D3DXVECTOR3 temp1;
	temp1.x = DesiredVelocity.x;
	temp1.z = DesiredVelocity.y;
    return (temp1 - 5);
  //}

  //return D3DXVECTOR3(0,0);
}

void CMovement_Algorithms::SetAlgorithms(int Flags)
{
	if(Flags == 0)
		WanderOff();
	if(Flags == 1)
		WanderOn();
	if(Flags == 2)
	{
		ArriveOn();
		//STarget = TargetPos;
	}
	if(Flags == 3)
		HideOn();

	m_iFlags;
}

void CMovement_Algorithms::SetEntitiesPursuing()
{
	for(unsigned int i = 0; i < m_vEntityData.size(); i++) //loop through all entities
	{
		//m_iFlags = m_vEntityData[i].m_iSteeringFlags;
		if(On(pursuit))
			m_vPursuers[i] = m_vEntityData[i]; //fill vector of entities that are pursuing
	}
}
void CMovement_Algorithms::SetEntitiesFlocking()
{
	for(unsigned int i = 0; i < m_vEntityData.size(); i++) //loop through all entities
	{
		//m_iFlags = m_vEntityData[i].m_iSteeringFlags;
		if(On(flock))
			m_vFlockers[i] = m_vEntityData[i]; //fill vector of entities that are flocking
	}
}

//---------------------- Calculate ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that point. 
//------------------------------------------------------------------------
SVector3D CMovement_Algorithms::Calculate(SteeringData Entity)
{ 
	SCurEntity = Entity;//set the current entity as determined by the for loop in Update(...)

	SVector3D force;

	if (On(obstacle_avoidance))
	{
		//m_pMovObject->GetAllObjects()
		force = ObstacleAvoidance() * m_dWeightObstacleAvoidance;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(evade))
	{
		//assert(m_pPersuer && "Evade target not assigned");
		//m_pPersuer //evades from this entity
		force = Evade(GetclosestPursuer()) * m_dWeightEvade; //former  Evade(m_pTarget)

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}


	if (On(flee))
	{
		force = Flee(SPersuer) * m_dWeightFlee; 

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	//these next three can be combined for flocking behavior (wander is
	//also a good behavior to add into this mix)
	if (On(separation))
	{
		//m_pMovObject->GetAllAgents()
		force = Separation() * m_dWeightSeparation;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(allignment))
	{
		//m_pMovObject->GetAllAgents()
		force = Alignment() * m_dWeightAlignment;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(cohesion))
	{
		//m_pMovObject->GetAllAgents()
		force = Cohesion() * m_dWeightCohesion;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(seek))
	{
		force = Seek(STarget) * m_dWeightSeek;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}


	if (On(arrive))
	{
		force = Arrive(STarget, m_eDeceleration) * m_dWeightArrive;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(wander))
	{
		force = Wander() * m_dWeightWander;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	if (On(pursuit))
	{
		//assert(m_pTarget && "pursuit target not assigned");
		//m_pTarget //chase this entity
		force = Pursuit(GetclosestEntityTypeBeingPursued()) * m_dWeightPursuit;

		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	//if (On(interpose))
	//{
	//	assert (m_pTargetAgent1 && m_pTargetAgent2 && "Interpose agents not assigned");
	//	
	//	force = Interpose(m_pTargetAgent1, m_pTargetAgent2) * m_dWeightInterpose;
	//	
	//	if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	//}
	
	if (On(hide))
	{
		//assert(m_pPersuer && "Hide target not assigned");
		//m_pPersuer, m_pMovObject->GetAllObjects()
		force = Hide(GetclosestPursuer()) * m_dWeightHide;
		if (!AccumulateForce(SSteeringForce, force)) return SSteeringForce;
	}

	return SSteeringForce;
}

SteeringData CMovement_Algorithms::GetclosestPursuer()
{
	double OldDistance = 9999999999;
	int ClosestPursuer = 0;
	for(unsigned int i = 0; i < m_vPursuers.size(); i++)  //loop through entities
	{
		//get distance to entity
		double Distance = SCurEntity.SPosition.Distance(m_vPursuers[i].SPosition); 
		if(Distance < OldDistance) //if new entity is closer than the previous entity
		{
			OldDistance = Distance; //set new closest distance
			ClosestPursuer =  i; //set new closest entity id
		}

	}
	return m_vPursuers[ClosestPursuer];
}

SteeringData CMovement_Algorithms::GetclosestEntityTypeBeingPursued()
{
	double OldDistance = 9999999999;
	int ClosestPursuer = 0;
	for(unsigned int i = 0; i < m_vPursuers.size(); i++) //loop through entities
	{
		if(SCurEntity.m_iPursuingType == m_vPursuers[i].m_iEntityType)
		{
			//get distance to entity
			double Distance = SCurEntity.SPosition.Distance(m_vPursuers[i].SPosition);
			if(Distance < OldDistance) //if new entity is closer than the previous entity
			{
				OldDistance = Distance; //set new closest distance
				ClosestPursuer =  i; //set new closest entity id
			}
		}
	}
	return m_vPursuers[ClosestPursuer];
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool CMovement_Algorithms::AccumulateForce(SVector3D &RunningTot, SVector3D ForceToAdd)
{
  
  //calculate how much steering force the vehicle has used so far
  double MagnitudeSoFar = RunningTot.Length();

  //calculate how much steering force remains to be used by this vehicle
  double MagnitudeRemaining = SCurEntity.m_dMaxForce - MagnitudeSoFar;

  //return false if there is no more force left to use
  if (MagnitudeRemaining <= 0.0) return false;

  //calculate the magnitude of the force we want to add
  double MagnitudeToAdd = ForceToAdd.Length();
  
  //if the magnitude of the sum of ForceToAdd and the running total
  //does not exceed the maximum force available to this vehicle, just
  //add together. Otherwise add as much of the ForceToAdd vector is
  //possible without going over the max.
  if (MagnitudeToAdd < MagnitudeRemaining)
  {
    RunningTot += ForceToAdd;
  }

  else
  {
    //add it to the steering force
    RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining); 
  }

  return true;
}


/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS
//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------

SVector3D CMovement_Algorithms::Seek(SVector3D TargetPos)
{
  SVector3D DesiredVelocity = Vec2DNormalize(TargetPos - SCurEntity.SPosition)
                            * SCurEntity.m_dMaxSpeed;

  return (DesiredVelocity - SCurEntity.SVelocity);
}


//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------

SVector3D CMovement_Algorithms::Flee(SVector3D TargetPos)
{

 //only flee if the target is within 'panic distance'. Work in distance
  //squared space.
//const double PanicDistanceSq = 100.0f * 100.0;
//  if (Vec2DDistanceSq(SCurEntity.SPosition, TargetPos) > PanicDistanceSq)
//  {
//    return SVector3D(0,0);
//  }
  
  //only flee if the target is within 'panic distance'. Work in distance
  //squared space.
  SVector3D DesiredVelocity = Vec2DNormalize(SCurEntity.SPosition - TargetPos) * SCurEntity.m_dMaxSpeed;

  return (DesiredVelocity - SCurEntity.SVelocity);



}


//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------

SVector3D CMovement_Algorithms::Arrive(SVector3D TargetPos, Deceleration deceleration)
{
  SVector3D ToTarget = TargetPos - SCurEntity.SPosition;

  //calculate the distance to the target
  double dist = ToTarget.Length();

  if (dist > 0)
  {
    //because Deceleration is enumerated as an int, this value is required
    //to provide fine tweaking of the deceleration..
    const double DecelerationTweaker = 0.3;

    //calculate the speed required to reach the target given the desired
    //deceleration
    double speed =  dist / ((double)deceleration * DecelerationTweaker);     

    //make sure the velocity does not exceed the max
    speed = min(speed, SCurEntity.m_dMaxSpeed);

    //from here proceed just like Seek except we don't need to normalize 
    //the ToTarget vector because we have already gone to the trouble
    //of calculating its length: dist. 
    SVector3D DesiredVelocity =  ToTarget * speed / dist;

    return (DesiredVelocity - SCurEntity.SVelocity);
  }

  return SVector3D(0,0);
}




//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
//const Entity_Class_Type1* evader
SVector3D CMovement_Algorithms::Pursuit(SteeringData evader)
{
	//if the evader is ahead and facing the agent then we can just seek
	//for the evader's current position.
	SVector3D ToEvader = evader.SPosition - SCurEntity.SPosition;

	double RelativeHeading = SCurEntity.SHeading.Dot(evader.SHeading);

	if ((ToEvader.Dot(SCurEntity.SHeading) > 0) && (RelativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return Seek(evader.SPosition);
	}

	//Not considered ahead so we predict where the evader will be.

	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	double LookAheadTime = ToEvader.Length() /(SCurEntity.m_dMaxSpeed + evader.m_dSpeed);

	//now seek to the predicted future position of the evader
	return Seek(evader.SPosition + evader.SVelocity * LookAheadTime);
}



//----------------------------- Evade ------------------------------------
//
//  similar to pursuit except the agent Flees from the estimated future
//  position of the pursuer
//------------------------------------------------------------------------
//const Entity_Class_Type1* pursuer
SVector3D CMovement_Algorithms::Evade(SteeringData pursuer)
{
	SVector3D ToPursuer = pursuer.SPosition - SCurEntity.SPosition;

	//uncomment the following two lines to have Evade only consider pursuers 
	//within a 'threat range'
	const double ThreatRange = 100.0;
	double temp1 = ThreatRange * ThreatRange; // range around the entity wanting to not be persued
	double temp = ToPursuer.LengthSq(); // distance from persuer to non-persuer
	if (temp > temp1)
	{
		return SVector3D();
		//return ToPursuer;
	}

	//the lookahead time is propotional to the distance between the pursuer
	//and the pursuer; and is inversely proportional to the sum of the
	//agents' velocities
	double LookAheadTime = ToPursuer.Length() / (SCurEntity.m_dMaxSpeed + pursuer.m_dSpeed);

	//now flee away from predicted future position of the pursuer
	SVector3D temp2 = Flee(pursuer.SPosition + pursuer.SVelocity *LookAheadTime);
	return temp2;
	//return Flee(pursuer->Position() + pursuer->Velocity() *LookAheadTime);
}



//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
SVector3D CMovement_Algorithms::Wander()
{ 
  //this behavior is dependent on the update rate, so this line must
  //be included when using time independent framerate.
  //double JitterThisTimeSlice = m_dWanderJitter * m_dTimeElapsed;
  double JitterThisTimeSlice = m_dWanderJitter;

  //first, add a small random vector to the target's position
  SWanderTarget += SVector3D(RandomClamped() * JitterThisTimeSlice,
                              RandomClamped() * JitterThisTimeSlice);

  //reproject this new vector back on to a unit circle
  SWanderTarget.Normalize();

  //increase the length of the vector to the same as the radius
  //of the wander circle
  SWanderTarget *= m_dWanderRadius;

  //move the target into a position WanderDist in front of the agent
  SVector3D target = SWanderTarget + SVector3D(m_dWanderDistance, 0);

  //project the target into world space
  SVector3D Target = PointToWorldSpace(target,SCurEntity.SHeading,SCurEntity.SHeading.Perp(),SCurEntity.SPosition);

  //and steer towards it
  return target - SCurEntity.SPosition; 
}



//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
//vector<CAstroid*>& obstacles
SVector3D CMovement_Algorithms::ObstacleAvoidance()
{
	//the detection box length is proportional to the agent's velocity
	m_dDBoxLength = SCurEntity.m_iMinDetectionBoxLength + (SCurEntity.m_dSpeed/SCurEntity.m_dMaxSpeed) * SCurEntity.m_iMinDetectionBoxLength;

	//tag all obstacles within range of the box for processing
	TagObstaclesWithinViewRange(SCurEntity,m_vAvoidObst, m_dDBoxLength);

	//this will keep track of the closest intersecting obstacle (CIB)
	bool IntersectingObstacle = false;
	SteeringObstacles ClosestIntersectingObstacle;

	//this will be used to track the distance to the CIB
	double DistToClosestIP = MaxDouble;

	//this will record the transformed local coordinates of the CIB
	SVector3D LocalPosOfClosestObstacle;

	for(unsigned int i = 0; i < m_vAvoidObst.size(); i++) //loop through entities
	{
		//m_vAvoidObst[i]

		//if the obstacle has been tagged within range proceed
		if (m_vAvoidObst[i].m_dTagged)
		{
			//calculate this obstacle's position in local space
			SVector3D LocalPos = PointToLocalSpace(m_vAvoidObst[i].SPosition,SCurEntity.SHeading,SCurEntity.SHeading.Perp(),SCurEntity.SPosition);

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPos.x >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				double ExpandedRadius = (m_vAvoidObst[i].m_dRadius * m_vAvoidObst[i].m_dScale) + (SCurEntity.m_dRadius * SCurEntity.m_dScale);
				//double ExpandedRadius = (*curOb)->Radius() + SCurEntity.m_dRadius;

				if (fabs(LocalPos.y) < ExpandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					double cX = LocalPos.x;
					double cY = LocalPos.y;

					//we only need to calculate the sqrt part of the above equation once
					double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

					double ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;

						IntersectingObstacle = true;
						ClosestIntersectingObstacle = m_vAvoidObst[i];
	
						LocalPosOfClosestObstacle = LocalPos;
					}         
				}
			}
		}
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	SVector3D SteeringForce;

	if (IntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
		m_dDBoxLength;

		//calculate the lateral force
		SteeringForce.y = (ClosestIntersectingObstacle.m_dRadius-
		LocalPosOfClosestObstacle.y)  * multiplier;   

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const double BrakingWeight = 0.2;

		SteeringForce.x = (ClosestIntersectingObstacle.m_dRadius - LocalPosOfClosestObstacle.x) * BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(SteeringForce,SCurEntity.SHeading,SCurEntity.SHeading.Perp());
}

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
//const vector<SteeringData*> &neighbors
SVector3D CMovement_Algorithms::Separation()
{  

	SVector3D SteeringForce;
	vector<SteeringData> neighbors = m_vFlockers;

	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target*** m_pPersuer m_pTarget
		if((neighbors[a].ID != SCurEntity.ID))// && (neighbors[a] != m_pPersuer))
		{ 
			SVector3D ToAgent = SCurEntity.SPosition - neighbors[a].SPosition;

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
		}
	}

	return SteeringForce;
}


//---------------------------- Alignment ---------------------------------
//
//  returns a force that attempts to align this agents heading with that
//  of its neighbors
//------------------------------------------------------------------------
//const vector<SteeringData*> &neighbors
SVector3D CMovement_Algorithms::Alignment()
{

	vector<SteeringData> neighbors = m_vFlockers;
	//used to record the average heading of the neighbors
	SVector3D AverageHeading;

	//used to count the number of vehicles in the neighborhood
	int    NeighborCount = 0;

	//iterate through all the tagged vehicles and sum their heading vectors  
	for (unsigned int a=0; a<neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough ***also make sure it doesn't
		//include any evade target ***
		if((neighbors[a].ID != SCurEntity.ID))// && (neighbors[a] != m_pTarget))
		{
			AverageHeading += neighbors[a].SHeading;

			++NeighborCount;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
		AverageHeading /= (double)NeighborCount;

		AverageHeading -= SCurEntity.SHeading;
	}

	return AverageHeading;
}

//-------------------------------- Cohesion ------------------------------
//
//  returns a steering force that attempts to move the agent towards the
//  center of mass of the agents in its immediate area
//------------------------------------------------------------------------
//const vector<SteeringData*> &neighbors
SVector3D CMovement_Algorithms::Cohesion()
{

	vector<SteeringData> neighbors = m_vFlockers;
	//first find the center of mass of all the agents
	SVector3D CenterOfMass, SteeringForce;

	int NeighborCount = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (unsigned int a=0; a<neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough ***also make sure it doesn't
		//include the evade target ***
		if((neighbors[a].ID != SCurEntity.ID))// && (neighbors[a] != m_pTarget))
		{
			CenterOfMass += neighbors[a].SPosition;

			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (double)NeighborCount;

		//now seek towards that position
		SteeringForce = Seek(CenterOfMass);
	}

	//the magnitude of cohesion is usually much larger than separation or
	//allignment so it usually helps to normalize it.
	return Vec2DNormalize(SteeringForce);
}



//--------------------------- Interpose ----------------------------------
//
//  Given two agents, this method returns a force that attempts to 
//  position the vehicle between them
//------------------------------------------------------------------------
//SVector3D Interpose(const Vehicle* AgentA, const Vehicle* AgentB)
//{
//  //first we need to figure out where the two agents are going to be at 
//  //time T in the future. This is approximated by determining the time
//  //taken to reach the mid way point at the current time at at max speed.
//  SVector3D MidPoint = (AgentA->Pos() + AgentB->Pos()) / 2.0;
//
//  double TimeToReachMidPoint = Vec2DDistance(m_pVehicle->Pos(), MidPoint) /
//                               m_pVehicle->MaxSpeed();
//
//  //now we have T, we assume that agent A and agent B will continue on a
//  //straight trajectory and extrapolate to get their future positions
//  SVector3D APos = AgentA->Pos() + AgentA->Velocity() * TimeToReachMidPoint;
//  SVector3D BPos = AgentB->Pos() + AgentB->Velocity() * TimeToReachMidPoint;
//
//  //calculate the mid point of these predicted positions
//  MidPoint = (APos + BPos) / 2.0;
//
//  //then steer to Arrive at it
//  return Arrive(MidPoint, fast);
//}


//--------------------------- Hide ---------------------------------------
//
// hunter = the entity(is pursuing) that the (*curOb) is going to try and hide from.
//
//------------------------------------------------------------------------
//the hunter is an entity persuing.
//const SteeringData* hunter, const vector<SteeringObstacles*>& obstacles
SVector3D CMovement_Algorithms::Hide(SteeringData hunter)
{
	double DistToClosest = MaxDouble;
	SVector3D BestHidingSpot;
	//SteeringObstacles curOb = m_vObstData.begin();
	SteeringObstacles closest;

	for(unsigned int i = 0; i < m_vObstData.size(); i++) //loop through entities
	{
		//calculate the position of the hiding spot for this obstacle
		SVector3D HidingSpot = GetHidingPosition(m_vObstData[i].SPosition,m_vObstData[i].m_dRadius,hunter.SPosition);

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		double dist = Vec2DDistanceSq(HidingSpot, SCurEntity.SPosition);

		if (dist < DistToClosest)
		{
			DistToClosest = dist;

			BestHidingSpot = HidingSpot;

			closest = m_vObstData[i];
		}  


	}//end while

	//if no suitable obstacles found then Evade the hunter
	if (DistToClosest == MaxFloat)
	{
		return Evade(hunter);
	}

	//else use Arrive on the hiding spot
	return Arrive(BestHidingSpot, fast);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position DistanceFromBoundary 
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
SVector3D CMovement_Algorithms::GetHidingPosition(const SVector3D& posOb, const double radiusOb, const SVector3D& posHunter)
{
  //calculate how far away the agent is to be from the chosen obstacle's
  //bounding radius
  const double DistanceFromBoundary = 30.0;
  double       DistAway    = radiusOb + DistanceFromBoundary;

  //calculate the heading toward the object from the hunter
  SVector3D ToOb = Vec2DNormalize(posOb - posHunter);
  
  //scale it to size and add to the obstacles position to get
  //the hiding spot.
  return (ToOb * DistAway) + posOb;
}


