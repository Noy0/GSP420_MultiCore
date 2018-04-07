#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
//------------------------------------------------------------------------
//
//  Name:   Transformations.h
//
//  Desc:   Functions for converting 2D vectors between World and Local
//          space.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>
#include "..\Utils\utils.h"
#include "SVector3D.h"
#include "C2DMatrix.h"
#include "Transformations.h"






//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position, orientation and scale,
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
//inline std::vector<SVector3D> WorldTransform1(std::vector<SPoint> &points,
//                                            const SVector3D   &pos,
//                                            const SVector3D   &forward,
//                                            const SVector3D   &side,
//                                            const SVector3D   &scale)
//{
//	//copy the original vertices into the buffer about to be transformed
//  std::vector<SPoint> TranSVector3Ds = points;
//  
//  //create a transformation matrix
//	C2DMatrix matTransform;
//	
//	//scale
//  if ( (scale.x != 1.0) || (scale.y != 1.0) )
//  {
//	  matTransform.Scale(scale.x, scale.y);
//  }
//
//	//rotate
//	matTransform.Rotate(forward, side);
//
//	//and translate
//	matTransform.Translate(pos.x, pos.y);
//	
//  //now transform the object's vertices
//	//matTransform.TransformSVector3Ds(TranSVector3Ds);
//	matTransform.TransformSPoints(TranSVector3Ds);
//
//  return TranSVector3Ds;
//}
//
////--------------------------- WorldTransform -----------------------------
////
////  given a std::vector of 2D vectors, a position and  orientation
////  this function transforms the 2D vectors into the object's world space
////------------------------------------------------------------------------
//inline std::vector<SVector3D> WorldTransform(std::vector<SVector3D> &points,
//                                 const SVector3D   &pos,
//                                 const SVector3D   &forward,
//                                 const SVector3D   &side)
//{
//	//copy the original vertices into the buffer about to be transformed
//	// std::vector<SVector3D> TranSVector3Ds = points;
//    //SVector3D TranSVector3Ds = points;
//  
//  //create a transformation matrix
//	C2DMatrix matTransform;
//
//	//rotate
//	matTransform.Rotate(forward, side);
//
//	//and translate
//	matTransform.Translate(pos.x, pos.y);
//	
//  //now transform the object's vertices
//  matTransform.TransformSVector3Ds(TranSVector3Ds);
//
//  return TranSVector3Ds;
//}

//--------------------- PointToWorldSpace --------------------------------
//
//  Transforms a point from the agent's local space into world space
//------------------------------------------------------------------------
inline SVector3D PointToWorldSpace(const SVector3D &point,
                                    const SVector3D &AgentHeading,
                                    const SVector3D &AgentSide,
                                    const SVector3D &AgentPosition)
{
	//make a copy of the point
	SVector3D TransPoint = point;
  
	//create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//and translate
	matTransform.Translate(AgentPosition.x, AgentPosition.y);
	
	//now transform the vertices
	matTransform.TransformSVector3Ds(TransPoint);

	return TransPoint;
}

//--------------------- VectorToWorldSpace --------------------------------
//
//  Transforms a vector from the agent's local space into world space
//------------------------------------------------------------------------
inline SVector3D VectorToWorldSpace(const SVector3D &vec,
                                   const SVector3D &AgentHeading,
                                   const SVector3D &AgentSide)
{
	//make a copy of the point
	SVector3D TransVec = vec;
  
	//create a transformation matrix
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//now transform the vertices
	matTransform.TransformSVector3Ds(TransVec);

	return TransVec;
}


//--------------------- PointToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline SVector3D PointToLocalSpace(const SVector3D &point,
										 SVector3D &AgentHeading,
										 SVector3D &AgentSide,
										 SVector3D &AgentPosition)
{
	
	//make a copy of the point
	SVector3D TransPoint = point;
  
	//create a transformation matrix
	C2DMatrix matTransform;

	double Tx = -AgentPosition.Dot(AgentHeading);
	double Ty = -AgentPosition.Dot(AgentSide);

	//create the transformation matrix
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	matTransform._31(Tx);			  matTransform._32(Ty);

	//now transform the vertices
	matTransform.TransformSVector3Ds(TransPoint);

	return TransPoint;
}

//--------------------- VectorToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline SVector3D VectorToLocalSpace(const SVector3D &vec,
                             const SVector3D &AgentHeading,
                             const SVector3D &AgentSide)
{ 

	//make a copy of the point
  SVector3D TransPoint = vec;
  
  //create a transformation matrix
	C2DMatrix matTransform;

  //create the transformation matrix
  matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
  matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	
  //now transform the vertices
  matTransform.TransformSVector3Ds(TransPoint);

  return TransPoint;
}

//-------------------------- Vec2DRotateAroundOrigin --------------------------
//
//  rotates a vector ang rads around the origin
//-----------------------------------------------------------------------------
//inline void Vec2DRotateAroundOrigin(SVector3D& v, double ang)
//{
//  //create a transformation matrix
//  C2DMatrix mat;
//
//  //rotate
//  mat.Rotate(ang);
//	
//  //now transform the object's vertices
//  mat.TransformSVector3Ds(v);
//}

//------------------------ CreateWhiskers ------------------------------------
//
//  given an origin, a facing direction, a 'field of view' describing the 
//  limit of the outer whiskers, a whisker length and the number of whiskers
//  this method returns a vector containing the end positions of a series
//  of whiskers radiating away from the origin and with equal distance between
//  them. (like the spokes of a wheel clipped to a specific segment size)
//----------------------------------------------------------------------------
//inline std::vector<SVector3D> CreateWhiskers(unsigned int  NumWhiskers,
//                                            double        WhiskerLength,
//                                            double        fov,
//                                            SVector3D      facing,
//                                            SVector3D      origin)
//{
//  //this is the magnitude of the angle separating each whisker
//  double SectorSize = fov/(double)(NumWhiskers-1);
//
//  std::vector<SVector3D> whiskers;
//  SVector3D temp;
//  double angle = -fov*0.5; 
//
//  for (unsigned int w=0; w<NumWhiskers; ++w)
//  {
//    //create the whisker extending outwards at this angle
//    temp = facing;
//    Vec2DRotateAroundOrigin(temp, angle);
//    whiskers.push_back(origin + WhiskerLength * temp);
//
//    angle+=SectorSize;
//  }
//
//  return whiskers;
//}


#endif