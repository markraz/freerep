//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Line.cpp - Author Jon Pry 

#include "Geom_Line.h"

Geom_Line::Geom_Line(Geom_Vec3 a, Geom_Vec3 b)
{
	m_a = a;
	m_b = b;	
}

Geom_Vec3 Geom_Line::ClosestPoint(Geom_Vec3 p)
{
	Geom_Vec3 u = m_b - m_a;
	Geom_Vec3 v = p - m_a;
	
	u.Normalize();
	double d = u * v;
	return (u * d) + m_a;
}

bool Geom_Line::IsOn(Geom_Vec3 p)
{
	Geom_Vec3 p2 = ClosestPoint(p);
	return p2 == p;	
}

Geom_Vec3 Geom_Line::Direction()
{
	return m_b - m_a;	
}