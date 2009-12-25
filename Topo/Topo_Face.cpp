//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face.cpp - Author Jon Pry 

#include "Topo_Face.h"

#include <stdio.h>

Topo_Face::Topo_Face()
{
	
}

Topo_Face::Topo_Face(const ICanAssociate* associate):ICanAssociate(associate)
{
	
}

double Topo_Face::Area()
{
	//TODO: throw not implemented
	return 0;	
}

void Topo_Face::Add(Topo_Edge *edge)
{
    m_edges.push_back(edge);
}

void Topo_Face::Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt))
{
	
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