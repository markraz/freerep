//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.cpp - Author Jon Pry 

#include "Geom_Plane.h"
#include "FreeREP.h"

Geom_Plane::Geom_Plane(Geom_Vec3 pnt1, Geom_Vec3 pnt2, Geom_Vec3 pnt3)
{
    Geom_Vec3 dir1 = pnt2 - pnt1;
    Geom_Vec3 dir2 = pnt3 - pnt1;

    dir1.Normalize();
    dir2.Normalize();

    m_pnt = pnt1;
    m_norm = dir1 ^ dir2;
    m_norm.Normalize();
}

Geom_Plane::Geom_Plane(Geom_Vec3 pnt, Geom_Vec3 norm)
{
    m_pnt = pnt; m_norm = norm;
}

Geom_Vec3 Geom_Plane::MapPoint(Geom_Vec3 pnt)
{
    double dot = m_norm.Dot(Geom_Vec3(0,0,1));
    //TODO: get confusion from settings class
    double confusion = 10e-9;
    if((dot > 1 - confusion && dot <  1 + confusion) || (dot > -1 -confusion && dot <  -1 + confusion))
        return pnt;

    Geom_Vec3 ydir = m_norm ^ Geom_Vec3(0,0,1);
    Geom_Vec3 xdir = m_norm ^ ydir;
    
    xdir = xdir * -1;

    //TODO: do this with matrix multiplication
    return Geom_Vec3(pnt * xdir,
                    pnt * ydir,
                    pnt * xdir);
}