//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face.h - Author Jon Pry 

#ifndef TOPO_FACE_H
#define TOPO_FACE_H

#include "Topo_Edge.h"
#include <list>
#include "Geom_Vec3.h"
#include "ICanCopy.h"
#include "ICanAssociate.h"

enum Topo_Face_Type
{
    TopoFacePlanar
};

class Topo_Face;

class Topo_Face: public ICanCopyWithTranslation, ICanAssociate
{
    Topo_Face_Type m_type;
    std::list<Topo_Edge*> m_edges;
    std::list<Topo_Edge*>::iterator m_edges_it;

public:
    Topo_Face();
    Topo_Face(const ICanAssociate *);

    void Add(Topo_Edge *edge);
    virtual void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt));


    void GetFirstWire(Topo_Wire **wire, EnumWireOrder *order);
    void GetNextWire(Topo_Wire **wire, EnumWireOrder *order);

    Topo_Edge* GetFirstEdge();
    Topo_Edge* GetNextEdge();

//Overrides from ICanCopyWithTranslation
    void* MakeTranslatedCopy(Geom_Vec3 dir) const;


};

#endif