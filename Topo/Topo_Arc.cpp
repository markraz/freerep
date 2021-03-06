//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Arc.cpp - Author Jon Pry 

#include "Topo_Arc.h"
#include "FreeREP.h"

#include <stdio.h>
#include <math.h>

Topo_Arc::Topo_Arc(const Geom_Ax2 &A, double radius, double start, double end)
{
    m_A = A; m_radius = radius, m_start = start; m_end = end;
}

void Topo_Arc::GetVertices(double dDeviation, void (*pRet)(const Geom_Vec3 &pt, const Geom_Vec3 &derivitive)) const
{
    int nsegs = 6;
    if(dDeviation < m_radius)
    {
    	double da = m_end - m_start;
    	int segs = floor(fabs(da) / 2 / acos((m_radius - dDeviation)/m_radius)+.5);
    	if(segs > nsegs)
    		nsegs = segs;	
    }
    
    //This improves stability with axis aligned spheres. Not a good general solution.
    //We need to make sure that we include the maxima as defined by the underlying surface
    //in the resultant vertices.
    
    if(nsegs%2)
    	nsegs++;
    
    for(int i=0; i <= nsegs; i++)
    {
        double u = 1.0 / nsegs * i;
        Geom_Vec3 pnt = GetPntAtU(u);
        Geom_Vec3 pnt2 = u < 1 - EPSILON?GetPntAtU(u+EPSILON):GetPntAtU(u-EPSILON);
        pRet(pnt,pnt2-pnt);
    }
}

Geom_Vec3 Topo_Arc::GetStart() const
{
    return GetPntAtU(0);
}

Geom_Vec3 Topo_Arc::GetEnd() const
{
    return GetPntAtU(1);
}

Geom_Vec3 Topo_Arc::GetPntAtU(double u) const
{
    double da = m_end - m_start;
    //if(da<0)da += 2 * M_PI;
    double ang = m_start + da * u;

	//TODO: add vector rotation stuff to vec3d
    Geom_Vec3 pnt = m_A.Location() + m_A.XDir() * (m_radius * cos(ang)) +  m_A.YDir() * (m_radius * sin(ang));
                   
    return pnt;
}

void* Topo_Arc::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Arc *narc = new Topo_Arc(*this);

    narc->m_A.SetLocation(m_A.Location() + dir);
    narc->m_parent = this;

    return narc;
}

bool Topo_Arc::HasNorm() const
{
    return true;
}

Geom_Vec3 Topo_Arc::Norm() const
{
    return m_A.ZDir();
}

Topo_Wire* Topo_Arc::Project(Geom_Plane &plane) const
{
	//TODO: implement me. Need support for elliptical arcs in case
	//we are not on a plane at least parallel to &plane
	
	//TODO: immediately important case is when we are perpendicular to 
	//plane and projection is a line
	
	Geom_Ax2 axis = m_A.Project(plane);
	Geom_Vec3 spnt = plane.MapPoint(GetStart());
	Geom_Vec3 epnt = plane.MapPoint(GetEnd());
	
	double start = atan2(spnt.m_y - axis.Location().m_y, spnt.m_x - axis.Location().m_x);
	double end = start + (m_end - m_start);
		
	
	return new Topo_Arc(axis,m_radius,start,end);
}

double Topo_Arc::IntegrateArc(double t) const
{
	return m_A.Location().m_y * m_radius * cos(t)  + (m_radius * m_radius/2) * ( -sin(t) * cos(t) + t);
}

double Topo_Arc::GetArea(EnumWireOrder order) const
{
	//TODO: test this
	
	//Equation for parametric arc is (x,y) = P + r (cos t, sin t)
	//Line integral symbolically Integral  -(P_y + r sin t ) ( - r sin t ) dt,
	//Solved: -P_y r cos t  + (r^2/2)( -sin t cos t + t)
	
	//Only works for counter clockwise arcs
	double start = m_start;
	double end = m_end;
	
	if(start < end)
	{
		double t = start;
		start = end;
		end = t;	
	}
	
	return IntegrateArc(start) - IntegrateArc(end);	
}

Geom_Ax2 Topo_Arc::GetAxis() const
{
	return m_A;	
}

void Topo_Arc::Print() 
{
	Geom_Vec3 loc = m_A.Location();
	Geom_Vec3 zdir = m_A.ZDir();
	Geom_Vec3 xdir = m_A.XDir();
	printf("Topo_Arc: loc: x: %lf, y: %lf, z: %lf, zdir: x: %lf, y: %lf, z: %lf, xdir: x: %lf, y: %lf, z: %lf, rad: %lf, start: %lf, end: %lf\n",
		loc.m_x,loc.m_y,loc.m_z,zdir.m_x,zdir.m_y,zdir.m_z,xdir.m_x,xdir.m_y,xdir.m_z,m_radius,m_start,m_end);
}

bool Topo_Arc::IsCoplanar(Geom_Plane &plane) const
{
	Geom_Vec3 c = plane.MapPoint(m_A.Location());
	double dot = m_A.ZDir() * plane.GetNorm();
	return ISZERO(c.m_z) && ISZERO(dot);	
}

double Topo_Arc::GetRadius() const
{
	return m_radius;	
}

Intersection Topo_Arc::Intersect(Topo_Wire *wire) const
{
	//TODO: implement me	
}

bool Topo_Arc::IsCoincident(Topo_Wire *wire) const 
{
	//TODO: implement me	
}

Topo_Wire* Topo_Arc::Merge(Topo_Wire *wire) const
{
	//TODO: implement me	
}