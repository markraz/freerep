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
	//TODO: figure out metric for conics
	double radius=1;//sphere->GetRadius();
		
	Geom_Vec3 start = a;
	Geom_Vec3 end = b;
	start.m_z = sqrt(radius * radius - start.m_x * start.m_x - start.m_y * start.m_y);
	end.m_z = sqrt(radius * radius - end.m_x * end.m_x - end.m_y * end.m_y);
	
	return (start - end).Norm();
}

Geom_Vec3 Topo_Face_Conic::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
	//double radius=sphere->GetRadius();
	//TODO: figure out subdivision point for conics
	return (a+b).Normalized();
}

void TopoFaceConicVertexMapper(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	Geom_Vec3 p = cone->GetPlane().MapPoint(pnt);
	MaxEdgeLengthVertexAbsorber(p,argh);
}

void Topo_Face_Conic::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	//double n = M_PI / acos((m_radius - dDeviation) / m_radius);
	//double s = 2 * dDeviation / tan(M_PI * (n-2)/ (2 * n));
	//TODO: figure out metric parameter for conics
	double s=1; 
	
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

