//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Spheric.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"
#include "FreeREP.h"

#include <math.h>

Topo_Face_Spheric::Topo_Face_Spheric()
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(Geom_Ax2 axis, double r)
{
	m_axis = axis;
	m_plane = Geom_Plane(axis);
	m_radius = r;
}

void Topo_Face_Spheric::ProjectPoint(const Geom_Vec3 &pnt, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	//TODO: rotate into the axis zdir
	double rt = sqrt(m_radius*m_radius - pnt.m_x *pnt.m_x);
	Geom_Vec3 p(rt * sin(pnt.m_y),rt * cos(pnt.m_y),pnt.m_x);
	p = Geom_Vec3(p.m_x,p.m_y,p.m_z);
	Geom_Vec3 norm = p.Normalized();
	pRet(p + m_plane.GetLocation(), norm);
}

const Topo_Face_Spheric *sphere;
void (*pTopoFaceSphericRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm);

void TopoFaceSphericVertexAbsorber(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	//Geom_Vec3 p = sphere->GetPlane().UnmapPoint(pnt);
	sphere->ProjectPoint(pnt,pTopoFaceSphericRet);
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
	
	return 0;//(start - end).Norm() * m_metric;
}

Geom_Vec3 Topo_Face_Spheric::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
 	return (a+b).Normalized() * m_radius;
}

void TopoFaceSphericVertexMapper(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = sphere->ParameterizePoint(pnt);
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

Geom_Vec3 Topo_Face_Spheric::ParameterizePoint(Geom_Vec3 p) const
{
	Geom_Line l = m_axis.GetLine();
	Geom_Vec3 cpnt = l.ClosestPoint(p);
	
	double mag = cpnt.Distance(m_axis.Location());
	double x=mag;
	if(!ISZERO(x))
	{
		Geom_Vec3 v = (cpnt  - m_axis.Location()).Normalized();
	
		x = (m_axis.ZDir().Normalized() * v) * mag;
	}
	
	Geom_Vec3 map = m_plane.MapPoint(p);
	
	double y = atan2(map.m_y,map.m_x);
	return Geom_Vec3(x,y,0);
}