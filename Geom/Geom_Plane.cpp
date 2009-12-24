//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.cpp - Author Jon Pry 

#include "Geom_Plane.h"
#include "FreeREP.h"

Geom_Plane::Geom_Plane(Geom_Vec3 pnt1, Geom_Vec3 pnt2, Geom_Vec3 pnt3)
{
    Geom_Vec3 dir1 = Geom_Vec3(pnt2.m_x-pnt1.m_x,pnt2.m_y-pnt1.m_y,pnt2.m_z-pnt1.m_z);
    Geom_Vec3 dir2 = Geom_Vec3(pnt3.m_x-pnt1.m_x,pnt3.m_y-pnt1.m_y,pnt3.m_z-pnt1.m_z);

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

    //TODO: do this with matrix multiplication
    return Geom_Vec3(pnt.m_x * xdir.m_x + pnt.m_y * xdir.m_y + pnt.m_z + xdir.m_z,
                    pnt.m_x * ydir.m_x + pnt.m_y * ydir.m_y + pnt.m_z + ydir.m_z,
                    pnt.m_x * m_norm.m_x + pnt.m_y * m_norm.m_y + pnt.m_z * m_norm.m_z);
}