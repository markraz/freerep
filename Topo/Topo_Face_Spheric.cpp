//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Spheric.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"
#include "FreeREP.h"

#include <math.h>
#include <stdio.h>

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
	Geom_Vec3 p((m_axis.XDir() * (rt * cos(pnt.m_y))) + 
				(m_axis.YDir() * rt * sin(pnt.m_y)) +
				(m_axis.ZDir() * pnt.m_x));
			//p = Geom_Vec3(p.m_x,p.m_y,p.m_z);
	Geom_Vec3 norm = p.Normalized()*-1;
	pRet(p + m_plane.GetLocation(), norm);
	//pRet(pnt,norm);
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
	//TODO: implement this. This is not a reasonable metric for the parametric space
	//return 1;
	return (a-b).Norm() * m_metric;
}

Geom_Vec3 Topo_Face_Spheric::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
 	return (a+b)*.5;
}

void Topo_Face_Spheric::TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvecs, Geom_Vec3 argh) const
{
	MaxEdgeLengthVertexAbsorber(nvecs[0],argh);
	MaxEdgeLengthVertexAbsorber(nvecs[1],argh);
	MaxEdgeLengthVertexAbsorber(nvecs[2],argh);
}

void Topo_Face_Spheric::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	double n = M_PI / acos((m_radius - dDeviation) / m_radius);
	double s = 2 * dDeviation / tan(M_PI * (n-2)/ (2 * n));
	
	//save the metric normalizer
	m_metric = 1/s;
	//m_metric /= 2;
	
	sphere = this;
	pTopoFaceSphericRet = pRet;
	SetupMaxEdgeLength(TopoFaceSphericVertexAbsorber,this);
	Topo_Face::Triangulate(dDeviation,0);
}

void *Topo_Face_Spheric::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Spheric *nface = new Topo_Face_Spheric(this);

	nface->m_plane.SetLocation(nface->m_plane.GetLocation() + dir);  
    return nface;
}

Geom_Vec3 Topo_Face_Spheric::ParameterizePoint(Geom_Vec3 p,Geom_Vec3 prev) const
{
	//printf("%lf,%lf,%lf\n",p.m_x,p.m_y,p.m_z);
	
	Geom_Line l = m_axis.GetLine();
	Geom_Vec3 cpnt = l.ClosestPoint(p);
	
	double mag = cpnt.Distance(m_axis.Location());
	double x=mag;
	if(!ISZERO(x))
	{
		Geom_Vec3 v = (cpnt  - m_axis.Location()).Normalized();
	
		x = ((m_axis.ZDir().Normalized() * v)<0?-1:1) * mag;
	}
	
	Geom_Vec3 map = m_plane.MapPoint(p);
	
	double y = atan2(map.m_y,map.m_x);
	if(ISZERO(map.m_y-map.m_x))
	{
		y = ParameterizePoint(prev,Geom_Vec3(0,0,0)).m_y;	
	}
	return Geom_Vec3(x,y,0);
}