//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane.cpp - Author Jon Pry 

#include "Geom_Plane.h"
#include "FreeREP.h"

Geom_Plane::Geom_Plane()
{
	m_axis = Geom_Ax2(Geom_Vec3(0,0,0),Geom_Vec3(0,0,1),Geom_Vec3(1,0,0));
}

Geom_Plane::Geom_Plane(Geom_Ax2 axis)
{
	m_axis = axis;	
}

Geom_Plane::Geom_Plane(double a, double b, double c, double d)
{	
	Geom_Vec3 norm = Geom_Vec3(a,b,c);
	
	double dot = norm.Normalized().Dot(Geom_Vec3(1,0,0));
    //TODO: get confusion from settings class
    double confusion = 10e-9;
    if((dot > 1 - confusion && dot <  1 + confusion) || (dot > -1 -confusion && dot <  -1 + confusion))
    {
    	m_axis = Geom_Ax2(norm.Normalized() *(d/norm.Norm()) ,norm.Normalized(),Geom_Vec3(0,1,0));
    }
	else
	{
    	Geom_Vec3 xdir = (norm ^ Geom_Vec3(1,0,0)).Normalized();
    	m_axis = Geom_Ax2(norm.Normalized() *(d/norm.Norm()) ,norm.Normalized(),xdir);
	}
}

Geom_Plane::Geom_Plane(Geom_Vec3 pnt1, Geom_Vec3 pnt2, Geom_Vec3 pnt3)
{
    Geom_Vec3 dir1 = pnt2 - pnt1;
    Geom_Vec3 dir2 = pnt3 - pnt1;

    dir1.Normalize();
    dir2.Normalize();

    Geom_Vec3 norm = (dir1 ^ dir2).Normalized();
    Geom_Vec3 xdir = (dir1 ^ norm).Normalized();
    
    m_axis = Geom_Ax2(pnt1,norm,xdir);
}

Geom_Plane::Geom_Plane(Geom_Vec3 pnt, Geom_Vec3 norm)
{
  	double dot = norm.Dot(Geom_Vec3(0,0,1));
    //TODO: get confusion from settings class
    double confusion = 10e-9;
    if((dot > 1 - confusion && dot <  1 + confusion) || (dot > -1 -confusion && dot <  -1 + confusion))
    {
    	m_axis = Geom_Ax2(pnt,norm,Geom_Vec3(1,0,0));
    }
	else
	{
    	Geom_Vec3 ydir = (norm ^ Geom_Vec3(0,0,1)).Normalized();
    	Geom_Vec3 xdir = (norm ^ ydir).Normalized();
    
    	xdir = xdir * -1;
    	
    	m_axis = Geom_Ax2(pnt,norm,xdir);
	}

}

Geom_Vec3 Geom_Plane::MapPoint(Geom_Vec3 pnt) const
{
	pnt = pnt - m_axis.Location();

    //TODO: do this with matrix multiplication
    Geom_Vec3 ret(pnt * m_axis.XDir(),
                    pnt * m_axis.YDir(),
                    pnt * m_axis.ZDir());
    if(ret.m_x * ret.m_x + ret.m_y * ret.m_y > 1)
    {
    	int x=0;
    	x++;	
    }     
    return ret;
}

Geom_Vec3 Geom_Plane::UnmapPoint(Geom_Vec3 pnt) const
{
	Geom_Vec3 xdir = m_axis.XDir();
	Geom_Vec3 ydir = m_axis.YDir();
	Geom_Vec3 norm = m_axis.ZDir();    
    //TODO: do this with matrix multiplication
    
    //This just transposes the matrix
    Geom_Vec3 nx(xdir.m_x,ydir.m_x,norm.m_x);
    Geom_Vec3 ny(xdir.m_y,ydir.m_y,norm.m_y);
    Geom_Vec3 nz(xdir.m_z,ydir.m_z,norm.m_z);
    
    pnt = pnt;// + m_pnt;
    Geom_Vec3 ret(pnt * nx,
                    pnt * ny,
                    pnt * nz);
    if(ret.m_x * ret.m_x + ret.m_y * ret.m_y > 1)
    {
    	int x=0;
    	x++;	
    }     
    return ret + m_axis.Location();
 }

Geom_Vec3 Geom_Plane::GetNorm() const
{
	return m_axis.ZDir();	
}

Geom_Vec3 Geom_Plane::GetLocation() const
{
	return m_axis.Location();	
}

void Geom_Plane::SetLocation(Geom_Vec3 pnt)
{
	m_axis.SetLocation(pnt);	
}