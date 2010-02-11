//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Line.cpp - Author Jon Pry 

#include "Topo_Line.h"

Topo_Line::Topo_Line(const Geom_Vec3& A, const Geom_Vec3& B)
{
    m_A = A; m_B = B;
}

Topo_Line::~Topo_Line()
{

}

void Topo_Line::GetVertices(double dDeviation, void (*pRet)(const Geom_Vec3 &pt, double u)) const
{
    pRet(m_A,0);
    pRet(m_B,1);
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