#include "CNode.h"


//-----------------------------------constructor-------------------------
//
//-----------------------------------------------------------------------
CNode::CNode()
{
	m_vPosition = Vector2D(400,300);
	m_dScale = .25;
	m_dRadius = m_dScale * 5;
	m_iIndex = 0;
}

//-------------------------------Update()--------------------------------
//
//-----------------------------------------------------------------------
bool CNode::Update()
{
	return true;
}

//---------------------WorldTransform--------------------------------
//
//	sets up a translation matrix for the sweeper according to its
//  scale, rotation and position. Returns the transformed vertices.
//-------------------------------------------------------------------
void CNode::WorldTransform(vector<SPoint> &node)
{
	//create the world transformation matrix
	C2DMatrix matTransform;
	
	//scale
	matTransform.Scale(m_dScale, m_dScale);
	
	//and translate
	matTransform.Translate(m_vPosition.x, m_vPosition.y);
	
	//now transform the ships vertices
	matTransform.TransformSPoints(node);
}

