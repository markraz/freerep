//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.cpp - Author Jon Pry 

#include "Topo_Face_Conic.h"
#include "Geom_Vec3.h"

#include <vector>

Topo_Face_Conic::Topo_Face_Conic(Topo_Arc *arc1, Topo_Arc *arc2)
{
    m_arc1 = arc1; m_arc2 = arc2;
}

std::vector<Geom_Vec3> vertex1;
std::vector<Geom_Vec3> vertex2;

void TopoFaceConicVertexAbsorber1(const Geom_Vec3& pnt, double u)
{
    vertex1.push_back(pnt);
}

void TopoFaceConicVertexAbsorber2(const Geom_Vec3& pnt, double u)
{
    vertex2.push_back(pnt);
}


void Topo_Face_Conic::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt))
{
//TODO: this is an unglorious hack
    vertex1.clear();
    vertex2.clear();

    m_arc1->GetVertices(dDeviation,TopoFaceConicVertexAbsorber1);
    m_arc2->GetVertices(dDeviation,TopoFaceConicVertexAbsorber2);

    if(vertex1.size() != vertex2.size() || !vertex1.size())
        return;

    bool reverse;
    if(vertex2.back().Distance(vertex1[0]) < vertex2[0].Distance(vertex1[0]))
        reverse = true;

    for(size_t i=1; i < vertex1.size(); i++)
    {
        Geom_Vec3 p1 = vertex1[i-1];
        Geom_Vec3 p2 = vertex1[i];

        Geom_Vec3 p3 = vertex2[i];
        Geom_Vec3 p4 = vertex2[i-1];

        if(reverse)
        {
            p3 = vertex2[vertex1.size() - i];
            p4 = vertex2[vertex1.size() - i-1];
        }

        pRet(p1);
        pRet(p2);
        pRet(p3);

        pRet(p1);
        pRet(p3);
        pRet(p4);
    }
}

void *Topo_Face_Conic::MakeTranslatedCopy(Geom_Vec3 dir) const
{
	//TODO: implement me with the rest of this class
	return 0;	
}