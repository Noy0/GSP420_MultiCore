#ifndef DD_STEERINGDATA_H
#define DD_STEERINGDATA_H

//#include "DDSteering.h"
#include "..\DynamicData.h"
#include "..\Movement\SVector3D.h"


struct DDSteeringData
{
	int ID;
	int SteeringType;
	int m_iEvadeID; //id of the entity that the current entity will be evading from.
	int m_iPursuitID; // id of the entity that the current entity will be persuing.
	double m_dSpeed;
	SVector3D SPosition;
	SVector3D SVelocity;
	//can get heading from velocity;
	SVector3D SHeading;
};

class DD_SteeringData: public DynamicData<vector<DDSteeringData>>
{
public:
	DD_SteeringData():DynamicData("Steering"){}
};

#endif
