//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.h - Author Jon Pry 

#ifndef GEOM_PLANE_H
#define GEOM_PLANE_H

#include "Geom_Vec3.h"
#include "Geom_Ax2.h"

bool Geom_Plane_Test();

class Geom_Plane
{
    Geom_Ax2 m_axis;
    
public:
	Geom_Plane();
    Geom_Plane(Geom_Vec3 pnt1, Geom_Vec3 pnt2, Geom_Vec3 pnt3);
    Geom_Plane(Geom_Vec3 pnt, Geom_Vec3 norm);
    Geom_Plane(Geom_Ax2 ax);

    Geom_Vec3 MapPoint(Geom_Vec3 p);
    Geom_Vec3 UnmapPoint(Geom_Vec3 p);
    Geom_Vec3 GetNorm() const;
    Geom_Vec3 GetLocation() const;
    void SetLocation(Geom_Vec3);
};

#endif