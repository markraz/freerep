//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.cpp - Author Jon Pry 

#include "Topo_Face_Conic.h"
#include "Geom_Vec3.h"
#include "Sub_MaxEdgeLength.h"
#include "Geom_Matrix.h"
#include "FreeREP.h"

#include <math.h>

Topo_Face_Conic::Topo_Face_Conic()
{
	
}

Topo_Face_Conic::Topo_Face_Conic(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

Topo_Face_Conic::Topo_Face_Conic(Geom_Ax2 axis, double r1, double r2, double length)
{
	m_axis = axis;
	m_plane = Geom_Plane(axis);
	m_radius_1 = r1;
	m_radius_2 = r2;
	m_length = length;
}

std::pair<Geom_Vec3,Geom_Vec3> Topo_Face_Conic::ProjectPoint(Geom_Vec3 pnt,Geom_Vec3 argh) const
{
	double xl = (pnt.m_x + m_length/2)/m_length;
	double rt = m_radius_1 + xl * (m_radius_2 - m_radius_1);
	Geom_Vec3 p((m_axis.XDir() * (rt * cos(pnt.m_y))) + 
				(m_axis.YDir() * rt * sin(pnt.m_y)) +
				(m_axis.ZDir() * pnt.m_x));

	Geom_Vec3 norm = Geom_Vec3(p.m_x,p.m_y,0).Normalized()*-1;
	
	Geom_Vec3 around = (norm ^ m_axis.ZDir()).Normalized();
	double dr = m_radius_2 - m_radius_1;
	Geom_Matrix m = Geom_Matrix::RotateAround(around,atan2(dr,m_length));
	
	return std::pair<Geom_Vec3,Geom_Vec3>(p + m_plane.GetLocation(), m.Multiply(norm).Normalized());
}

const Topo_Face_Conic *cone;
void (*pTopoFaceConicRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm);

void TopoFaceConicVertexAbsorber(const Geom_Vec3&pnt,const Geom_Vec3&argh)
{
	std::pair<Geom_Vec3,Geom_Vec3> pair = cone->ProjectPoint(pnt,argh);
	pTopoFaceConicRet(pair.first,pair.second);
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
	//return 0;
	double x1 = -a.m_x/m_length + .5;
	double x2 = -b.m_x/m_length + .5;
	
	double r1 = x1 * m_radius_2 + (1-x1) * m_radius_1;
	double r2 = x2 * m_radius_2 + (1-x2) * m_radius_1;
	
	double r=r1;
	if(r2 > r1)
		r = r2;
		
	double n = M_PI / acos((r - m_deviation) / r);
	//double s = 2 * m_deviation / tan(M_PI * (n-2)/ (2 * n));
	double rads = 2 * M_PI / n;
	
	return fabs(a.m_y - b.m_y) / rads;
}

Geom_Vec3 Topo_Face_Conic::Subdivide(Geom_Vec3 a, Geom_Vec3 b) const
{
	Geom_Vec3 ret = (a+b)/2;
	
	return ret;
	
}

void Topo_Face_Conic::TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvecs, Geom_Vec3 argh) const
{
	MaxEdgeLengthVertexAbsorber(nvecs[0],argh);
	MaxEdgeLengthVertexAbsorber(nvecs[1],argh);
	MaxEdgeLengthVertexAbsorber(nvecs[2],argh);
}

void Topo_Face_Conic::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
{
	m_deviation = dDeviation;
	
	cone = this;
	pTopoFaceConicRet = pRet;
	SetupMaxEdgeLength(TopoFaceConicVertexAbsorber,this);
	Topo_Face::Triangulate(dDeviation,0);
}

void *Topo_Face_Conic::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Conic *nface = new Topo_Face_Conic(this);

	nface->m_plane.SetLocation(nface->m_plane.GetLocation() + dir);  
    return nface;
}

bool Topo_Face_Conic::Contains(Topo_Wire *wire)
{
	
}

Geom_Vec3 Topo_Face_Conic::ParameterizePoint(Geom_Vec3 p,Geom_Vec3 derivitive) const
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
		map = map - derivitive;
		y = atan2(map.m_y,map.m_x);	
	}
	return Geom_Vec3(x,y,0);
}