//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.cpp - Author Jon Pry 

#include "Geom_Plane.h"
#include "FreeREP.h"

Geom_Plane::Geom_Plane()
{
	m_pnt = Geom_Vec3(0,0,0);
	m_norm = Geom_Vec3(0,0,1);
}

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
	pnt = pnt - m_pnt;
    double dot = m_norm.Dot(Geom_Vec3(0,0,1));
    //TODO: get confusion from settings class
    double confusion = 10e-9;
    if((dot > 1 - confusion && dot <  1 + confusion) || (dot > -1 -confusion && dot <  -1 + confusion))
        return pnt;

    Geom_Vec3 ydir = (m_norm ^ Geom_Vec3(0,0,1)).Normalized();
    Geom_Vec3 xdir = (m_norm ^ ydir).Normalized();
    
    xdir = xdir * -1;

    //TODO: do this with matrix multiplication
    Geom_Vec3 ret(pnt * xdir,
                    pnt * ydir,
                    pnt * m_norm);
    if(ret.m_x * ret.m_x + ret.m_y * ret.m_y > 1)
    {
    	int x=0;
    	x++;	
    }     
    return ret;
}

Geom_Vec3 Geom_Plane::UnmapPoint(Geom_Vec3 pnt)
{
    double dot = m_norm.Dot(Geom_Vec3(0,0,1));
    //TODO: get confusion from settings class
    double confusion = 10e-9;
    if((dot > 1 - confusion && dot <  1 + confusion) || (dot > -1 -confusion && dot <  -1 + confusion))
        return pnt;

    Geom_Vec3 ydir = (m_norm ^ Geom_Vec3(0,0,1)).Normalized();
    Geom_Vec3 xdir = (m_norm ^ ydir).Normalized();
    
    xdir = xdir * -1;

    //TODO: do this with matrix multiplication
    
    //This just transposes the matrix
    Geom_Vec3 nx(xdir.m_x,ydir.m_x,m_norm.m_x);
    Geom_Vec3 ny(xdir.m_y,ydir.m_y,m_norm.m_y);
    Geom_Vec3 nz(xdir.m_z,ydir.m_z,m_norm.m_z);
    
    pnt = pnt;// + m_pnt;
    Geom_Vec3 ret(pnt * nx,
                    pnt * ny,
                    pnt * nz);
    if(ret.m_x * ret.m_x + ret.m_y * ret.m_y > 1)
    {
    	int x=0;
    	x++;	
    }     
    return ret + m_pnt;
 }

Geom_Vec3 Geom_Plane::GetNorm() const
{
	return m_norm;	
}

Geom_Vec3 Geom_Plane::GetLocation() const
{
	return m_pnt;	
}

void Geom_Plane::SetLocation(Geom_Vec3 pnt)
{
	m_pnt = pnt;	
}