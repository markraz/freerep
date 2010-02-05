//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Spheric.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"

#include <math.h>

Topo_Face_Spheric::Topo_Face_Spheric()
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(Geom_Plane plane, double r)
{
	m_plane = plane;
	m_radius = r;
}

void Topo_Face_Spheric::ProjectPoint(const Geom_Vec3 &pnt, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	Geom_Vec3 norm = (pnt - m_plane.GetLocation()).Normalized();
	pRet((norm * m_radius) + m_plane.GetLocation(), norm);
}

const Topo_Face_Spheric *sphere;
void (*pTopoFaceSphericRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm);

void TopoFaceSphericVertexAbsorber(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = sphere->GetPlane().UnmapPoint(pnt);
	sphere->ProjectPoint(p,pTopoFaceSphericRet);
}

double Topo_Face_Spheric::GetRadius() const
{
	return m_radius;	
}

double Topo_Face_Spheric::MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const
{
	Geom_Vec3 start = a;
	Geom_Vec3 end = b;
	start.m_z = sqrt(m_radius * m_radius - start.m_x * start.m_x - start.m_y * start.m_y);
	end.m_z = sqrt(m_radius * m_radius - end.m_x * end.m_x - end.m_y * end.m_y);
	
	return (start - end).Norm() * m_metric;
}

Geom_Vec3 Topo_Face_Spheric::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
 	return (a+b).Normalized() * m_radius;
}

void TopoFaceSphericVertexMapper(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = sphere->GetPlane().MapPoint(pnt);
	MaxEdgeLengthVertexAbsorber(p,argh);
}

void Topo_Face_Spheric::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	double n = M_PI / acos((m_radius - dDeviation) / m_radius);
	double s = 2 * dDeviation / tan(M_PI * (n-2)/ (2 * n));
	
	//save the metric normalizer
	m_metric = 1/s;
	
	sphere = this;
	pTopoFaceSphericRet = pRet;
	SetupMaxEdgeLength(TopoFaceSphericVertexAbsorber,this);
	Topo_Face::Triangulate(dDeviation,TopoFaceSphericVertexMapper);
}

void *Topo_Face_Spheric::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Spheric *nface = new Topo_Face_Spheric(this);

	nface->m_plane.SetLocation(nface->m_plane.GetLocation() + dir);  
    return nface;
}

