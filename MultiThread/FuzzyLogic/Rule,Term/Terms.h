#pragma once

class Terms
{  
public:

  virtual ~Terms(){}

  virtual Terms* Clone()const = 0;

  virtual double      GetDOM()const = 0;;

  virtual void       ClearDOM()=0;

  virtual void       ORwithDOM(double val)= 0;;

   
};