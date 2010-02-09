//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Line.h - Author Jon Pry 

#ifndef GEOM_LINE_H
#define GEOM_LINE_H

#include "Geom_Vec3.h"

class Geom_Line
{
	Geom_Vec3 m_a;
	Geom_Vec3 m_b;
public:
	Geom_Line(Geom_Vec3 a, Geom_Vec3 b);
	
	Geom_Vec3 ClosestPoint(Geom_Vec3 p);
};

#endif 
