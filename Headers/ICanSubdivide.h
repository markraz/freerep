//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//ICanSubdivide.h - Author Jon Pry 

#ifndef ICANSUBDIVIDE_H
#define ICANSUBDIVIDE_H

#include "Geom_Vec3.h"

class ICanSubdivide
{
public:
	virtual double MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const=0;
	virtual Geom_Vec3 Subdivide(Geom_Vec3 a, Geom_Vec3 b) const=0;
};

#endif 
