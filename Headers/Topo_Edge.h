//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Edge.h - Author Jon Pry 

#ifndef TOPO_EDGE_H
#define TOPO_EDGE_H

#include "Topo_Face.h"
#include "Topo_Shape.h"
#include "Topo_Wire.h"
#include "EnumWireOrder.h"
#include "Geom_Vec3.h"
#include "Geom_Plane.h"
#include "ICanCopy.h"
#include "ICanAssociate.h"

#include <list>

class PlacedWire
{
public:
    Topo_Wire *m_wire;
    EnumWireOrder m_order;

    PlacedWire(Topo_Wire *wire, EnumWireOrder order){m_wire = wire; m_order = order;}
};

class Topo_Face;

class Topo_Edge: public Topo_Shape, public ICanCopyWithTranslation, public ICanAssociate
{
	std::list<Topo_Face*> m_parents;
	std::list<Topo_Face*>::iterator m_parents_it;
	
    std::list<PlacedWire*> m_wires;
    std::list<PlacedWire*>::iterator m_wires_it;
    bool m_closed;

public:
    Topo_Edge();
    Topo_Edge(const ICanAssociate* parent);

    void Add(Topo_Wire *w);
    void Add(Topo_Wire *w, EnumWireOrder order);
    void GetVertices(double dDeviation, void (*)(const Geom_Vec3 &pt));
    void Reverse();

    void GetFirstWire(Topo_Wire **wire, EnumWireOrder *order);
    void GetNextWire(Topo_Wire **wire, EnumWireOrder *order);
    
    Topo_Face* GetFirstParent();
    Topo_Face* GetNextParent();
    void AddParent(Topo_Face*);
    
    double Area(Topo_Face *face);

//Overrides from ICanCopyWithTranslation
    void* MakeTranslatedCopy(Geom_Vec3 dir) const;

};

#endif