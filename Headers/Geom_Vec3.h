//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Pnt.h - Author Jon Pry 

#ifndef GEOM_PNT_H
#define GEOM_PNT_H

bool Geom_Vec3_Test();

class Geom_Vec3
{
public:
    double m_x,m_y,m_z;

    Geom_Vec3(double x, double y, double z);
    Geom_Vec3();
    
  	Geom_Vec3 operator+(Geom_Vec3) const;
   	Geom_Vec3 operator-(Geom_Vec3) const;
   	bool operator==(Geom_Vec3) const;
   	Geom_Vec3 operator*(double) const;
   	double operator*(Geom_Vec3) const;
   	Geom_Vec3 operator/(double) const;
   	Geom_Vec3 operator^(Geom_Vec3) const; 

    bool IsEqual(Geom_Vec3 pnt) const;
    bool ExactEquals(Geom_Vec3 pnt) const;
    double Distance(Geom_Vec3 pnt) const;
    
    Geom_Vec3 Added(Geom_Vec3 pnt) const;
    Geom_Vec3 Subtracted(Geom_Vec3 pnt) const;
    Geom_Vec3 Multiplied(double v) const;
    Geom_Vec3 Divided(double v) const;
    void Multiply(double v);
    
    double Norm() const;
    void Normalize();
    Geom_Vec3 Normalized() const;
    
    Geom_Vec3 Crossed(Geom_Vec3) const;
    double Dot(Geom_Vec3) const;
 
 };

#endif