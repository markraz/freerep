//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Edge.cpp - Author Jon Pry 

#include "Topo_Edge.h"

Topo_Edge::Topo_Edge()
{
    m_closed = false;
}

Topo_Edge::Topo_Edge(const ICanAssociate* associate):ICanAssociate(associate)
{
    m_closed = false;
}

void Topo_Edge::Add(Topo_Wire *w, EnumWireOrder order)
{
    PlacedWire *pwire = new PlacedWire(w,order);
    m_wires.push_back(pwire);
}

void Topo_Edge::Add(Topo_Wire *w)
{
    PlacedWire *firstwire = m_wires.front();
    PlacedWire *lastwire = m_wires.back();

    if(m_wires.empty())
    {
        m_wires.push_back(new PlacedWire(w,AFirst));
        return;
    }

    bool OnStart=false;

    if(firstwire->m_order == AFirst)
    {
        if(w->GetEnd() == firstwire->m_wire->GetStart())
        {
            m_wires.push_front(new PlacedWire(w,AFirst));
            OnStart=true;
        }
        else if(w->GetStart() == firstwire->m_wire->GetStart())
        {
            m_wires.push_front(new PlacedWire(w,BFirst));
            OnStart=true;
        }
    }
    else
    {
        if(w->GetEnd() == firstwire->m_wire->GetEnd())
        {
            m_wires.push_front(new PlacedWire(w,AFirst));
            OnStart=true;
        }
        else if(w->GetStart() == firstwire->m_wire->GetEnd())
        {
            m_wires.push_front(new PlacedWire(w,BFirst));
            OnStart=true;
        }
    }

    bool OnEnd=false;

    if(lastwire->m_order == BFirst)
    {
        if(w->GetEnd() == lastwire->m_wire->GetStart())
        {
            if(!OnStart)
                m_wires.push_back(new PlacedWire(w,BFirst));
            OnEnd=true;
        }
        else if(w->GetStart() == lastwire->m_wire->GetStart())
        {
            if(!OnStart)
                m_wires.push_back(new PlacedWire(w,AFirst));
            OnEnd=true;
        }
    }
    else
    {
        if(w->GetEnd() == lastwire->m_wire->GetEnd())
        {
            if(!OnStart)
                m_wires.push_back(new PlacedWire(w,BFirst));
            OnEnd=true;
        }
        else if(w->GetStart() == lastwire->m_wire->GetEnd())
        {
            if(!OnStart)
                m_wires.push_back(new PlacedWire(w,AFirst));
            OnEnd=true;
        }
    }

    if(OnStart && OnEnd)
        m_closed = true;
}

std::list<Geom_Vec3> vertices;

void EdgeVertexAbsorber(const Geom_Vec3 &pt, double u)
{
    vertices.push_back(pt);
}

void Topo_Edge::GetVertices(double dDeviation, void (*pRet)(const Geom_Vec3 &pt))
{
    bool GotStart;
    Geom_Vec3 start;
    Geom_Vec3 last;

    std::list<PlacedWire*>::iterator it;
    for(it = m_wires.begin(); it != m_wires.end(); ++it)
    {
        vertices.clear();
        PlacedWire* pwire = *it;
        Topo_Wire *twire = pwire->m_wire;
        twire->GetVertices(dDeviation,EdgeVertexAbsorber);

        if(pwire->m_order == AFirst)
        {
            if(!GotStart)
            {
                start = vertices.front();
                GotStart = true;
                pRet(start);
                last = start;
            }
            std::list<Geom_Vec3>::iterator it2 = vertices.begin();
            for(it2++; it2 != vertices.end(); it2++)
            {
                last = *it2;
                pRet(last);
            }
        }
        else
        {
            if(!GotStart)
            {
                start = vertices.back();
                GotStart = true;
                pRet(start);
                last = start;
            }
            std::list<Geom_Vec3>::iterator it2 = vertices.end();
            it2--;
            for(it2--;; it2--)
            {
                last = *it2;
                pRet(last);
                if(it2 == vertices.begin())
                    break;
            }
        }
    }
    pRet(start);
}

void Topo_Edge::Reverse()
{
    m_wires.reverse();

    std::list<PlacedWire*>::iterator it = m_wires.begin();
    for(; it != m_wires.end(); it++)
    {
        PlacedWire *wire = *it;
        if(wire->m_order == AFirst)
            wire->m_order = BFirst;
        else
            wire->m_order = AFirst;
    }
}

void* Topo_Edge::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Edge *nedge = new Topo_Edge(this);
    std::list<PlacedWire*>::const_iterator it;
    for(it = m_wires.begin(); it != m_wires.end(); it++)
    {
        PlacedWire *pwire = *it;
        nedge->m_wires.push_back(new PlacedWire((Topo_Wire*)pwire->m_wire->MakeTranslatedCopy(dir),pwire->m_order));
    }

    return nedge;
}

void Topo_Edge::GetFirstWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    m_wires_it = m_wires.begin();

    return GetNextWire(ppwire,porder);
}

void Topo_Edge::GetNextWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    if(m_wires_it != m_wires.end())
    {
        PlacedWire* pwire = *m_wires_it;
        *ppwire = pwire->m_wire;
        *porder = pwire->m_order;

        m_wires_it++;
    }
    else
        *ppwire = 0;
}

Topo_Face* Topo_Edge::GetFirstParent()
{
	m_parents_it = m_parents.begin();
	return GetNextParent();
}

Topo_Face* Topo_Edge::GetNextParent()
{	
	if(m_parents_it != m_parents.end())
		return *m_parents_it++;
	return 0;
}

void Topo_Edge::AddParent(Topo_Face* parent)
{
	m_parents.push_back(parent);	
}

/*
Geom_Plane Topo_Edge::GetPlane()
{
    Geom_Vec3 pnts[3];
    Geom_Vec3 norm;
    int npoints = 0;

    Topo_Wire* wire;
    EnumWireOrder order;
    GetFirstWire(&wire,&order);
    while(wire)
    {
        pnts[npoints++] = wire->GetStart();
        if(npoints > 1 && pnts[npoints-1] == pnts[npoints-2])
            npoints--;

        if(wire->HasNorm())
        {
            norm = wire->Norm();
            return Geom_Plane(pnts[0],norm);
        }
        if(npoints>2)
        {
            return Geom_Plane(pnts[0],pnts[1],pnts[2]);
        }
        GetNextWire(&wire,&order);
    }
    
    //TODO: throw some kind of exception or make null or default plane
}*/