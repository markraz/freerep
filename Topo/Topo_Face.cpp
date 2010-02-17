//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face.cpp - Author Jon Pry 

#include "Topo_Face.h"
#include "Tess_Planar.h"

#include <stdio.h>
#include <vector>

Topo_Face::Topo_Face()
{
	
}

Topo_Face::Topo_Face(const ICanAssociate* associate):ICanAssociate(associate)
{
	
}

Geom_Plane Topo_Face::GetPlane() const
{
	return m_plane;
}

double Topo_Face::Area()
{
	//TODO: throw not implemented
	return 0;	
}

void Topo_Face::Add(Topo_Edge *edge)
{
    m_edges.push_back(edge);
    edge->AddParent(this);
}

void Topo_Face::Add(Topo_Edge *edge, bool inside)
{
	double area = edge->Area(this);
	if((area < 0 && !inside) || (area > 0 && inside))
		edge->Reverse();
		
    m_edges.push_back(edge);
    edge->AddParent(this);
}


std::vector<std::vector<Geom_Vec3> > topo_face_vertices;
int topo_face_current_edge;

void topo_face_vertex_absorber(const Geom_Vec3 &pnt)
{
    topo_face_vertices[topo_face_current_edge].push_back(pnt);
}

void Topo_Face::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const
{
	if(!m_edges.size())
		return;
	
    Geom_Plane plane = GetPlane();

    topo_face_vertices.clear();
    topo_face_vertices.resize(m_edges.size());

    topo_face_current_edge = 0;
    std::list<Topo_Edge*>::const_iterator it;
    for(it = m_edges.begin(); it != m_edges.end(); it++)
    {
        Topo_Edge *edge = *it;
        edge->GetVertices(dDeviation, topo_face_vertex_absorber);

        topo_face_current_edge++;
    }

    size_t total_size=0;
    int *sizes = new int[topo_face_vertices.size()];
    for(size_t i=0; i < topo_face_vertices.size(); i++)
    {
        sizes[i] = topo_face_vertices[i].size()-2;
        total_size += sizes[i];
    }

    double (*vertices)[2] = new double[total_size + 1][2];
    double (*uvertices)[3] = new double[total_size + 1][3];
    int (*triangles)[3] = new int[total_size * 8][3];
    size_t cvertex = 1;
    for(size_t i=0; i < topo_face_vertices.size(); i++)
    {
        for(size_t j = 0; j < (unsigned int)sizes[i]; j++)
        {
            Geom_Vec3 p = topo_face_vertices[i][j];
            //TODO: project onto the faces plane

            uvertices[cvertex][0] = p.m_x;
            uvertices[cvertex][1] = p.m_y;
            uvertices[cvertex][2] = p.m_z;

            Geom_Vec3 mp = plane.MapPoint(p);
            vertices[cvertex][0] = mp.m_x;
            vertices[cvertex][1] = mp.m_y;

            cvertex++;
        }
    }

#ifdef TRI_DEBUG
    printf("%d\n", (int)topo_face_vertices.size());

    for(size_t i=0; i < topo_face_vertices.size();i++)
    {
        printf("%d\n",sizes[i]);
    }

    for(size_t i=1; i < cvertex; i++)
    {
        printf("%lf,%lf  %lf,%lf,%lf \n",vertices[i][0],vertices[i][1],uvertices[i][0],uvertices[i][1],uvertices[i][2]);
    }
#endif

    int ntris = triangulate_polygon(topo_face_vertices.size(),sizes, vertices,triangles);

    for(int i=0; i < ntris; i++)
    {
        pRet(Geom_Vec3(uvertices[triangles[i][0]][0],uvertices[triangles[i][0]][1],uvertices[triangles[i][0]][2]),plane.GetNorm());
        pRet(Geom_Vec3(uvertices[triangles[i][1]][0],uvertices[triangles[i][1]][1],uvertices[triangles[i][1]][2]),plane.GetNorm());
        pRet(Geom_Vec3(uvertices[triangles[i][2]][0],uvertices[triangles[i][2]][1],uvertices[triangles[i][2]][2]),plane.GetNorm());
    }

    delete sizes;
    delete vertices;
    delete triangles;
    delete uvertices;
}

void Topo_Face::GetFirstWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    m_edges_it = m_edges.begin();

    Topo_Wire *wire=0;
    EnumWireOrder order;
    while(!wire)
    {
        if(m_edges_it != m_edges.end())
        {
            Topo_Edge* edge = *m_edges_it;
            edge->GetFirstWire(&wire,&order);
            if(wire)
                break;
        }
        else
            break;
        m_edges_it++;
    }
    *ppwire = wire;
    *porder = order;
}

void Topo_Face::GetNextWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    Topo_Wire *wire=0;
    EnumWireOrder order;

    if(m_edges_it != m_edges.end())
    {
       Topo_Edge* edge = *m_edges_it;
       edge->GetNextWire(&wire,&order);
    }

    while(!wire)
    {
        m_edges_it++;

        if(m_edges_it != m_edges.end())
        {
            Topo_Edge* edge = *m_edges_it;
            edge->GetFirstWire(&wire,&order);
            if(wire)
                break;
        }
        else
            break;
    }
    *ppwire = wire;
    *porder = order;
}

Topo_Edge* Topo_Face::GetFirstEdge()
{
    m_edges_it = m_edges.begin();
    return GetNextEdge();
}

Topo_Edge* Topo_Face::GetNextEdge()
{
    if(m_edges_it != m_edges.end())
        return *m_edges_it++;
    return 0;
}

Topo_Wire* Topo_Face::Project(Topo_Wire *wire)
{
	//Default operation is to project the wire onto m_plane by flattening the Z coordinate
	return wire->Project(m_plane);
}

bool Topo_Face::Contains(Topo_Wire *wire)
{
	//TODO: should never get called, throw exception
	return false;	
}