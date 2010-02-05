//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.cpp - Author Jon Pry 

#include "Topo_Face_Conic.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"

#include <math.h>

Topo_Face_Conic::Topo_Face_Conic()
{
	
}

Topo_Face_Conic::Topo_Face_Conic(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

Topo_Face_Conic::Topo_Face_Conic(Geom_Ax2 axis, double r1, double r2, double length)
{
	m_plane = Geom_Plane(axis);
	m_radius_1 = r1;
	m_radius_2 = r2;
	m_length = length;
}

void Topo_Face_Conic::ProjectPoint(const Geom_Vec3 &pnt, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	//TODO: figure out projection for conics
	Geom_Vec3 norm = (pnt - m_plane.GetLocation()).Normalized();
	pRet(pnt, norm);
}

const Topo_Face_Conic *cone;
void (*pTopoFaceConicRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm);

void TopoFaceConicVertexAbsorber(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = cone->GetPlane().UnmapPoint(pnt);
	cone->ProjectPoint(p,pTopoFaceConicRet);
}

double Topo_Face_Conic::GetRadius1() const
{
	return m_radius_1;	
}

double Topo_Face_Conic::GetRadius2() const
{
	return m_radius_2;	
}

double Topo_Face_Conic::GetLength() const
{
	return m_length;	
}


double Topo_Face_Conic::MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const
{
	double x1 = -a.m_x/m_length + .5;
	double x2 = -b.m_x/m_length + .5;
	
	double r1 = x1 * m_radius_2 + (1-x1) * m_radius_1;
	double r2 = x2 * m_radius_2 + (1-x2) * m_radius_1;
	
	double r=r1;
	if(r2 > r1)
		r = r2;
		
	double n = M_PI / acos((r - m_deviation) / r);
	double s = 2 * m_deviation / tan(M_PI * (n-2)/ (2 * n));
	
	return fabs(a.m_y - b.m_y) / s;
}

Geom_Vec3 Topo_Face_Conic::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
	Geom_Vec3 ret = (a+b)/2;
	
	double x = a.m_x/m_length + .5;
	double r = x * m_radius_2 + (1-x) * m_radius_1;
	
	//ret.m_z = sqrt(r*r - ret.m_y * ret.m_y);
	
	int y=0;
	y++;
	
	return ret;
	
}

void TopoFaceConicVertexMapper(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = cone->GetPlane().MapPoint(pnt);
	MaxEdgeLengthVertexAbsorber(p,argh);
}

void Topo_Face_Conic::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	m_deviation = dDeviation;
	
	cone = this;
	pTopoFaceConicRet = pRet;
	SetupMaxEdgeLength(TopoFaceConicVertexAbsorber,this);
	Topo_Face::Triangulate(dDeviation,TopoFaceConicVertexMapper);
}

void *Topo_Face_Conic::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Conic *nface = new Topo_Face_Conic(this);

	nface->m_plane.SetLocation(nface->m_plane.GetLocation() + dir);  
    return nface;
}

