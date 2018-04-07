#ifndef DD_STEERINGOBSTACLES_H
#define DD_STEERINGOBSTACLES_H

#include "..\DynamicData.h"
#include "..\Movement\SVector3D.h"


// all variables that Movement algorithms need access to
struct SteeringObstacles
{
	SVector3D	SPosition;
	double		m_dRadius;
	double		m_dScale;
	bool		m_dTagged;
	int			m_iID;
};

class DD_SteeringObstacles: public DynamicData<vector<SteeringObstacles>>
{
public:
	DD_SteeringObstacles():DynamicData("SteeringObstacles"){}
};

#endif