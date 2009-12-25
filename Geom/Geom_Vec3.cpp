//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Pnt.cpp - Author Jon Pry 

#include "Geom_Vec3.h"
#include "FreeREP.h"

#include <math.h>

Geom_Vec3::Geom_Vec3(double x, double y, double z)
{
    m_x = x; m_y = y; m_z = z;
}

Geom_Vec3::Geom_Vec3()
{
}

bool Geom_Vec3::IsEqual(Geom_Vec3 pnt) const
{
    double dx = pnt.m_x - m_x;
    double dy = pnt.m_y - m_y;
    double dz = pnt.m_z - m_z;

    return dx*dx+dy*dy+dz*dz < FreeREP::CONFUSION2;
}

double Geom_Vec3::Distance(Geom_Vec3 pnt) const
{
    double dx = pnt.m_x - m_x;
    double dy = pnt.m_y - m_y;
    double dz = pnt.m_z - m_z;

    return sqrt(dx * dx + dy * dy + dz * dz);
} 

Geom_Vec3 Geom_Vec3::Added(Geom_Vec3 pnt) const
{
	return Geom_Vec3(m_x + pnt.m_x, m_y + pnt.m_y, m_z + pnt.m_z);
}

Geom_Vec3 Geom_Vec3::Subtracted(Geom_Vec3 pnt) const
{
	return Geom_Vec3(m_x - pnt.m_x, m_y - pnt.m_y, m_z - pnt.m_z);
}

Geom_Vec3 Geom_Vec3::Multiplied(double v) const
{
	return Geom_Vec3(m_x * v, m_y * v, m_z * v);
}

void Geom_Vec3::Multiply(double v) 
{
	m_x *= v; m_y *= v; m_z *= v;
}

Geom_Vec3 Geom_Vec3::Divided(double v) const
{
	return Multiplied(1 / v);
}

double Geom_Vec3::Dot(Geom_Vec3 dir) const
{
	return m_x * dir.m_x + m_y * dir.m_y + m_z * dir.m_z;
}

Geom_Vec3 Geom_Vec3::Crossed(Geom_Vec3 dir) const
{
	return Geom_Vec3(-m_z * dir.m_y + m_y * dir.m_z, m_z * dir.m_x - m_x * dir.m_z, -m_y * dir.m_x + m_x * dir.m_y);
}

void Geom_Vec3::Normalize()
{
	Multiply(1/Norm());
}

Geom_Vec3 Geom_Vec3::Normalized() const
{
	return Multiplied(1/Norm());
}

double Geom_Vec3::Norm() const
{
	return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);	
}


bool Geom_Vec3::operator==(Geom_Vec3 pnt) const{
	return IsEqual(pnt);
} 

Geom_Vec3 Geom_Vec3::operator+(Geom_Vec3 pnt) const{
	return Added(pnt);
} 

Geom_Vec3 Geom_Vec3::operator-(Geom_Vec3 pnt) const{
	return Subtracted(pnt);
} 

Geom_Vec3 Geom_Vec3::operator*(double v) const{
	return Multiplied(v);
}

Geom_Vec3 Geom_Vec3::operator/(double v) const{
	return Divided(v);
}

double Geom_Vec3::operator*(Geom_Vec3 pnt) const{
	return Dot(pnt);
} 
 
Geom_Vec3 Geom_Vec3::operator^(Geom_Vec3 pnt) const{
	return Crossed(pnt);
} 
