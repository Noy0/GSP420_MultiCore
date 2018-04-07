#ifndef CNODE_H
#define CNODE_H

#include "..\Utils\utils.h"
#include "Common\2D\Vector2D.h"
#include "..\Movement\C2DMatrix.h"

class CNode
{
private:
	//its position in the world
	Vector2D		m_vPosition;
	
	double			m_dRadius;

	//the scale of the astroid when drawn
	double			m_dScale;

public:
	//ctor
	CNode();

	int m_iIndex;
	
	bool			Update();
	//used to transform the sweepers vertices prior to rendering
	void			WorldTransform(vector<SPoint> &node);

	//-------------------accessor functions
	inline Vector2D		Position(){return m_vPosition;}
	inline void			SetPos(Vector2D NewPosition){m_vPosition = NewPosition;}
	inline double		Scale()const{return m_dScale;}
	inline double		Radius()const{return m_dRadius;}
};

#endif