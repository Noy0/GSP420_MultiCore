#ifndef S3DVECTOR_H
#define S3DVECTOR_H
//------------------------------------------------------------------------
//
//  Name:   SVector3D.h
//
//  Desc:   2D vector struct
//
//  Author: Modified by Derek Hofferber
//
//------------------------------------------------------------------------
#include <math.h>
#include <windows.h>
#include <iosfwd>
#include <limits>
#include "..\Utils\utils.h"

//
//Funktastic
//
struct SVector3D
{
  double x;
  double y;
  //double z;

  SVector3D():x(0.0),y(0.0){}
  //SVector3D():x(0.0),y(0.0), z(0.0){}
  SVector3D(double a, double b):x(a),y(b){}
 // SVector3D(double a, double b, double c):x(a),y(b),z(c){}

  //sets x and y to zero
  //void Zero(){x=0.0; y=0.0; z=0.0;}
  void Zero(){x=0.0; y=0.0;}

  //returns true if both x and y are zero
  bool isZero()const{return (x*x + y*y) < MinDouble;}

  //returns the length of the vector
  inline double    Length()const;

  //returns the squared length of the vector (thereby avoiding the sqrt)
  inline double    LengthSq()const;

  inline void      Normalize();

  inline void	   UnNormalize();

  inline double    Dot(const SVector3D& v2)const;

  //returns positive if v2 is clockwise of this vector,
  //negative if anticlockwise (assuming the Y axis is pointing down,
  //X axis to right like a Window app)
  inline int       Sign(const SVector3D& v2)const;

  //returns the vector that is perpendicular to this one.
  inline SVector3D  Perp()const;

  //adjusts x and y so that the length of the vector does not exceed max
  inline void      Truncate(double max);

  //returns the distance between this vector and th one passed as a parameter
  inline double    Distance(const SVector3D &v2)const;

  //squared version of above.
  inline double    DistanceSq(const SVector3D &v2)const;

  inline void      Reflect(const SVector3D& norm);

  //returns the vector that is the reverse of this vector
  inline SVector3D  GetReverse()const;


  //we need some overloaded operators
  const SVector3D& operator+=(const SVector3D &rhs)
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  const SVector3D& operator-=(const SVector3D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  const SVector3D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;

    return *this;
  }

  const SVector3D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;

    return *this;
  }

  bool operator==(const SVector3D& rhs)const
  {
    return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
  }

  bool operator!=(const SVector3D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y);
  }
  
};

//-----------------------------------------------------------------------some more operator overloads
inline SVector3D operator*(const SVector3D &lhs, double rhs);
inline SVector3D operator*(double lhs, const SVector3D &rhs);
inline SVector3D operator-(const SVector3D &lhs, const SVector3D &rhs);
inline SVector3D operator+(const SVector3D &lhs, const SVector3D &rhs);
inline SVector3D operator/(const SVector3D &lhs, double val);
//std::ostream& operator<<(std::ostream& os, const SVector3D& rhs);
//std::ifstream& operator>>(std::ifstream& is, SVector3D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//
//------------------------------------------------------------------------
inline double SVector3D::Length()const
{
  return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline double SVector3D::LengthSq()const
{
  return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline double SVector3D::Dot(const SVector3D &v2)const
{
  return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum {clockwise = 1, anticlockwise = -1};

inline int SVector3D::Sign(const SVector3D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline SVector3D SVector3D::Perp()const
{
  return SVector3D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline double SVector3D::Distance(const SVector3D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline double SVector3D::DistanceSq(const SVector3D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void SVector3D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void SVector3D::Reflect(const SVector3D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline SVector3D SVector3D::GetReverse()const
{
  return SVector3D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void SVector3D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}

//------------------------- UnNormalize ------------------------------------
//
//  unnormalizes a 2D Vector
//------------------------------------------------------------------------
inline void SVector3D::UnNormalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x *= vector_length;
    this->y *= vector_length;
  }
}


//------------------------------------------------------------------------non member functions

inline SVector3D Vec2DNormalize(const SVector3D &v)
{
  SVector3D vec = v;

  double vector_length = vec.Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}


inline double Vec2DDistance(const SVector3D &v1, const SVector3D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const SVector3D &v1, const SVector3D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const SVector3D& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const SVector3D& v)
{
  return (v.x*v.x + v.y*v.y);
}


inline SVector3D POINTStoVector(const POINTS& p)
{
  return SVector3D(p.x, p.y);
}

inline SVector3D POINTtoVector(const POINT& p)
{
  return SVector3D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const SVector3D& v)
{
  POINTS p;
  p.x = (short)v.x;
  p.y = (short)v.y;

  return p;
}

inline POINT VectorToPOINT(const SVector3D& v)
{
  POINT p;
  p.x = (long)v.x;
  p.y = (long)v.y;

  return p;
}



//------------------------------------------------------------------------operator overloads
inline SVector3D operator*(const SVector3D &lhs, double rhs)
{
  SVector3D result(lhs);
  result *= rhs;
  return result;
}

inline SVector3D operator*(double lhs, const SVector3D &rhs)
{
  SVector3D result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
inline SVector3D operator-(const SVector3D &lhs, const SVector3D &rhs)
{
  SVector3D result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  
  return result;
}

//overload the + operator
inline SVector3D operator+(const SVector3D &lhs, const SVector3D &rhs)
{
  SVector3D result(lhs);
  result.x += rhs.x;
  result.y += rhs.y;
  
  return result;
}

//overload the / operator
inline SVector3D operator/(const SVector3D &lhs, double val)
{
  SVector3D result(lhs);
  result.x /= val;
  result.y /= val;

  return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(SVector3D &pos, int MaxX, int MaxY)
{
  if (pos.x > MaxX) {pos.x = 0.0;}

  if (pos.x < 0)    {pos.x = (double)MaxX;}

  if (pos.y < 0)    {pos.y = (double)MaxY;}

  if (pos.y > MaxY) {pos.y = 0.0;}
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(SVector3D p,
                            SVector3D top_left,
                            SVector3D bot_rgt)
{
  return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(SVector3D p,
                         SVector3D top_left,
                         SVector3D bot_rgt)
{
  return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(SVector3D p, int left, int top, int right, int bottom)
{
  return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(SVector3D posFirst,
                                 SVector3D facingFirst,
                                 SVector3D posSecond,
                                 double    fov)
{
  SVector3D toTarget = Vec2DNormalize(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}

#endif
