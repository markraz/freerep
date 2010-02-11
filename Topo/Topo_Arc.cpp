//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Arc.cpp - Author Jon Pry 

#include "Topo_Arc.h"
#include <math.h>

Topo_Arc::Topo_Arc(const Geom_Ax2 &A, double radius, double start, double end)
{
    m_A = A; m_radius = radius, m_start = start; m_end = end;
}

void Topo_Arc::GetVertices(double dDeviation, void (*pRet)(const Geom_Vec3 &pt, double u)) const
{
    int nsegs = 5;
    if(dDeviation < m_radius)
    {
    	double da = m_end - m_start;
    	int segs = floor(fabs(da) / 2 / acos((m_radius - dDeviation)/m_radius)+.5);
    	if(segs > nsegs)
    		nsegs = segs;	
    }
    
    for(int i=0; i <= nsegs; i++)
    {
        double u = 1.0 / nsegs * i;
        Geom_Vec3 pnt = GetPntAtU(u);
        pRet(pnt,u);
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
	
	Geom_Ax2 axis = m_A.Project(plane);
	
	
	//TODO: even if we are coplanar, this probably doesn't work
	return new Topo_Arc(axis,m_radius,m_start,m_end);
}

double Topo_Arc::GetArea(EnumWireOrder order) const
{
	//TODO: implement me
	return 0;	
}