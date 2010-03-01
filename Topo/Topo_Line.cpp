//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Line.cpp - Author Jon Pry 

#include "Topo_Line.h"
#include "FreeREP.h"

#include "stdio.h"

Topo_Line::Topo_Line(const Geom_Vec3& A, const Geom_Vec3& B)
{
    m_A = A; m_B = B;
}

Topo_Line::~Topo_Line()
{

}

void Topo_Line::GetVertices(double dDeviation, void (*pRet)(const Geom_Vec3 &pt, const Geom_Vec3 &derivitive)) const
{
    pRet(m_A,(m_A - m_B).Normalized());
    pRet(m_B,(m_B - m_A).Normalized());
}

Geom_Vec3 Topo_Line::GetStart() const
{
    return m_A;
}

Geom_Vec3 Topo_Line::GetEnd() const
{
    return m_B;
}

void* Topo_Line::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Line *nline = new Topo_Line(*this);

    nline->m_A = nline->m_A + dir;
    nline->m_B = nline->m_B + dir;

    nline->m_parent = this;

    return nline;
}

bool Topo_Line::HasNorm() const
{
    return false;
}

Geom_Vec3 Topo_Line::Norm() const
{
    return Geom_Vec3(0,0,1);
}

Topo_Wire* Topo_Line::Project(Geom_Plane &plane) const
{
	Geom_Vec3 a;
	Geom_Vec3 b;
	a = plane.MapPoint(m_A);	
	b = plane.MapPoint(m_B);

	return new Topo_Line(a,b);	
}


double Topo_Line::GetArea(EnumWireOrder order) const
{
	return (m_A.m_x * m_B.m_y - m_A.m_y * m_B.m_x)/2;
}

Geom_Line Topo_Line::GetLine()
{
	return Geom_Line(m_A,m_B);	
}

void Topo_Line::Print()
{
	printf("Topo_Line: x1: %lf, y1: %lf, z1: %lf, x2: %lf, y2: %lf, z2: %lf\n",
		m_A.m_x,m_A.m_y,m_A.m_z,m_B.m_x,m_B.m_y,m_B.m_z);	
}

bool Topo_Line::IsCoplanar(Geom_Plane &plane) const
{
	Geom_Vec3 a = plane.MapPoint(m_A);
	Geom_Vec3 b = plane.MapPoint(m_B);
	
	return ISZERO(a.m_z) && ISZERO(b.m_z);	
}

Intersection Topo_Line::Intersect(Topo_Wire *wire) const
{
	Topo_Line *line = dynamic_cast<Topo_Line*>(wire);
	if(line)
	{
		double ua = ((line->m_B.m_x-line->m_A.m_x) * (m_A.m_y - line->m_A.m_y) - (line->m_B.m_y - line->m_A.m_y) * (m_A.m_x - line->m_A.m_x)) /
					((line->m_B.m_y-line->m_A.m_y) * (m_B.m_x - m_A.m_x) - (line->m_B.m_x - line->m_A.m_x) * (m_B.m_y - m_A.m_y));
					
		double ub = ((m_B.m_x - m_A.m_x) * (m_A.m_y - line->m_A.m_y) - (m_B.m_y - m_A.m_y) * (m_A.m_x - line->m_A.m_x)) / 
					((line->m_B.m_y - line->m_A.m_y) * (m_B.m_x - m_A.m_x) - (line->m_B.m_x - line->m_A.m_x) * (m_B.m_y - m_A.m_y));
					
		Geom_Vec3 pnt(m_A.m_x + ua * (m_B.m_x - m_A.m_x),m_A.m_y + ua * (m_B.m_y - m_A.m_y),0);
		
		bool exists = ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1;
		return Intersection(exists,pnt,ua,ub);			
		
	}
	
	//TODO: implement the other cases	
}