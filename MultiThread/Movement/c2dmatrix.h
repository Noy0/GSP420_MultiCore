#ifndef C2DMATRIX_H
#define C2DMATRIX_H
//------------------------------------------------------------------------
//
//	Name: C2DMatrix.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Matrix class for 'Smart Sweeper' example from the book Game AI 
//  Programming with Neural Nets and Genetic Algorithms.. 
//
//------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "SVector3D.h"
#include "..\Utils\utils.h"

using namespace std;


class C2DMatrix
{
private:
  
  struct S2DMatrix
  {

	  double _11, _12, _13;
	  double _21, _22, _23;
	  double _31, _32, _33;

    S2DMatrix()
	  {
		  _11=0.0f; _12=0.0f; _13=0.0f;
		  _21=0.0f; _22=0.0f; _23=0.0f;
		  _31=0.0f; _32=0.0f; _33=0.0f;
	  }

    friend ostream &operator<<(ostream& os, const S2DMatrix &rhs)
	  {
		  os << "\n" << rhs._11 << "  " << rhs._12 << "  " << rhs._13;

		  os << "\n" << rhs._21 << "  " << rhs._22 << "  " << rhs._23;

		  os << "\n" << rhs._31 << "  " << rhs._32 << "  " << rhs._33;

		  return os;
	  }
  };

  S2DMatrix m_Matrix;

  //multiplies m_Matrix with mIn
  inline void  MatrixMultiply(S2DMatrix &mIn);

public:

  C2DMatrix()
  {
    //initialize the matrix to an identity matrix
    Identity();
  }

  //create an identity matrix
  inline void Identity();
  
  //create a transformation matrix
  inline void	Translate(double x, double y);

  //create a scale matrix
  inline void	Scale(double xScale, double yScale);

  //create a rotation matrix
  inline void	Rotate(double rotation);

  //create a rotation matrix from a fwd and side 2D vector
  inline void  Rotate(const SVector3D &fwd, const SVector3D &side);

   //applys a transformation matrix to a std::vector of points
  inline void TransformSPoints(vector<SPoint> &vPoints);	
   
  //inline void TransformSVector3Ds(std::vector<SVector3D> &vPoint);
  //applys a transformation matrix to a point
  inline void TransformSVector3Ds(SVector3D &vPoint);

  void _11(double val){m_Matrix._11 = val;}
  void _12(double val){m_Matrix._12 = val;}
  void _13(double val){m_Matrix._13 = val;}

  void _21(double val){m_Matrix._21 = val;}
  void _22(double val){m_Matrix._22 = val;}
  void _23(double val){m_Matrix._23 = val;}

  void _31(double val){m_Matrix._31 = val;}
  void _32(double val){m_Matrix._32 = val;}
  void _33(double val){m_Matrix._33 = val;}

};


//multiply two matrices together
inline void C2DMatrix::MatrixMultiply(S2DMatrix &mIn)
{
	C2DMatrix::S2DMatrix mat_temp;
  
  //first row
  mat_temp._11 = (m_Matrix._11*mIn._11) + (m_Matrix._12*mIn._21) + (m_Matrix._13*mIn._31);
  mat_temp._12 = (m_Matrix._11*mIn._12) + (m_Matrix._12*mIn._22) + (m_Matrix._13*mIn._32);
  mat_temp._13 = (m_Matrix._11*mIn._13) + (m_Matrix._12*mIn._23) + (m_Matrix._13*mIn._33);

  //second
  mat_temp._21 = (m_Matrix._21*mIn._11) + (m_Matrix._22*mIn._21) + (m_Matrix._23*mIn._31);
  mat_temp._22 = (m_Matrix._21*mIn._12) + (m_Matrix._22*mIn._22) + (m_Matrix._23*mIn._32);
  mat_temp._23 = (m_Matrix._21*mIn._13) + (m_Matrix._22*mIn._23) + (m_Matrix._23*mIn._33);

  //third
  mat_temp._31 = (m_Matrix._31*mIn._11) + (m_Matrix._32*mIn._21) + (m_Matrix._33*mIn._31);
  mat_temp._32 = (m_Matrix._31*mIn._12) + (m_Matrix._32*mIn._22) + (m_Matrix._33*mIn._32);
  mat_temp._33 = (m_Matrix._31*mIn._13) + (m_Matrix._32*mIn._23) + (m_Matrix._33*mIn._33);

  m_Matrix = mat_temp;
}

//applies a 2D transformation matrix to a std::vector of SVector3Ds
inline void C2DMatrix::TransformSPoints(vector<SPoint> &vPoints)
{
  for (unsigned int i=0; i< vPoints.size(); ++i)
  {
    double tempX =(m_Matrix._11*vPoints[i].x) + (m_Matrix._21*vPoints[i].y) + (m_Matrix._31);

    double tempY = (m_Matrix._12*vPoints[i].x) + (m_Matrix._22*vPoints[i].y) + (m_Matrix._32);
  
    vPoints[i].x = tempX;

    vPoints[i].y = tempY;

  }
}

//applies a 2D transformation matrix to a single SVector3D
inline void C2DMatrix::TransformSVector3Ds(SVector3D &vPoint)
{

  double tempX =(m_Matrix._11*vPoint.x) + (m_Matrix._21*vPoint.y) + (m_Matrix._31);

  double tempY = (m_Matrix._12*vPoint.x) + (m_Matrix._22*vPoint.y) + (m_Matrix._32);
  
  vPoint.x = tempX;

  vPoint.y = tempY;
}



//create an identity matrix
inline void C2DMatrix::Identity()
{
  m_Matrix._11 = 1; m_Matrix._12 = 0; m_Matrix._13 = 0;

  m_Matrix._21 = 0; m_Matrix._22 = 1; m_Matrix._23 = 0;

  m_Matrix._31 = 0; m_Matrix._32 = 0; m_Matrix._33 = 1;

  }

//create a transformation matrix
inline void C2DMatrix::Translate(double x, double y)
{
  S2DMatrix mat;
  
  mat._11 = 1; mat._12 = 0; mat._13 = 0;
  
  mat._21 = 0; mat._22 = 1; mat._23 = 0;
  
  mat._31 = x;    mat._32 = y;    mat._33 = 1;
  
  //and multiply
  MatrixMultiply(mat);
}

//create a scale matrix
inline void C2DMatrix::Scale(double xScale, double yScale)
{
  C2DMatrix::S2DMatrix mat;
  
  mat._11 = xScale; mat._12 = 0; mat._13 = 0;
  
  mat._21 = 0; mat._22 = yScale; mat._23 = 0;
  
  mat._31 = 0; mat._32 = 0; mat._33 = 1;
  
  //and multiply
  MatrixMultiply(mat);
}


//create a rotation matrix
inline void C2DMatrix::Rotate(double rot)
{
  C2DMatrix::S2DMatrix mat;

  double Sin = sin(rot);
  double Cos = cos(rot);
  
  mat._11 = Cos;  mat._12 = Sin; mat._13 = 0;
  
  mat._21 = -Sin; mat._22 = Cos; mat._23 = 0;
  
  mat._31 = 0; mat._32 = 0;mat._33 = 1;
  
  //and multiply
  MatrixMultiply(mat);
}


//create a rotation matrix from a 2D vector
inline void C2DMatrix::Rotate(const SVector3D &fwd, const SVector3D &side)
{
  C2DMatrix::S2DMatrix mat;
  
  mat._11 = fwd.x;  mat._12 = fwd.y; mat._13 = 0;
  
  mat._21 = side.x; mat._22 = side.y; mat._23 = 0;
  
  mat._31 = 0; mat._32 = 0;mat._33 = 1;
  
  //and multiply
  MatrixMultiply(mat);
}


#endif
