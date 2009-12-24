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
    //TODO: calculate number of segments based on deviation parameter
    for(int i=0; i < 21; i++)
    {
        double u = 1.0 / 20 * i;
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
    if(da<0)da += 2 * M_PI;
    double ang = m_start + da * u;

	//TODO: add vector rotation stuff to vec3d
    return Geom_Vec3(m_A.Location().m_x + m_radius * sin(ang) * m_A.XDir().Dot(Geom_Vec3(1,0,0)) +
                                         m_radius * cos(ang) * m_A.XDir().Dot(Geom_Vec3(0,1,0)),
                    m_A.Location().m_y + m_radius * cos(ang) * m_A.YDir().Dot(Geom_Vec3(0,1,0)) +
                    m_A.Location().m_y + m_radius * sin(ang) * m_A.YDir().Dot(Geom_Vec3(1,0,0)),
                    m_A.Location().m_z);
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