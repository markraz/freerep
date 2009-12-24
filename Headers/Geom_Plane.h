//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.h - Author Jon Pry 

#ifndef GEOM_PLANE_H
#define GEOM_PLANE_H

#include "Geom_Vec3.h"

class Geom_Plane
{
    Geom_Vec3 m_pnt;
    Geom_Vec3 m_norm;
public:
    Geom_Plane(Geom_Vec3 pnt1, Geom_Vec3 pnt2, Geom_Vec3 pnt3);
    Geom_Plane(Geom_Vec3 pnt, Geom_Vec3 norm);

    Geom_Vec3 MapPoint(Geom_Vec3 p);
};

#endif