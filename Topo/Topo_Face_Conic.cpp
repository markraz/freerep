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


void Topo_Face_Conic::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const
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
        
        //TODO: these normals are not good enough for per vertex 
        //normals

		Geom_Vec3 v1 = p1 - p2;
		Geom_Vec3 v2 = p1 - p3;
		
		Geom_Vec3 norm = v2 ^ v1;
		norm.Normalize();

        pRet(p1,norm);
        pRet(p2,norm);
        pRet(p3,norm);

        pRet(p1,norm);
        pRet(p3,norm);
        pRet(p4,norm); 
    }
}

void *Topo_Face_Conic::MakeTranslatedCopy(Geom_Vec3 dir) const
{
	//TODO: implement me with the rest of this class
	return 0;	
}