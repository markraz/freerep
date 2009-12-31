//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face.h - Author Jon Pry 

#ifndef TOPO_FACE_H
#define TOPO_FACE_H

#include "Topo_Shape.h"
#include "Topo_Edge.h"
#include "Geom_Vec3.h"
#include "Geom_Plane.h"
#include "ICanCopy.h"
#include "ICanAssociate.h"
#include "ICanTriangulate.h"

#include <list>

enum Topo_Face_Type
{
	TopoFaceConic,
    TopoFacePlanar
};

class Topo_Face;

class Topo_Face: public Topo_Shape, public ICanCopyWithTranslation, public ICanAssociate, public ICanTriangulate
{
protected:
    Topo_Face_Type m_type;
    std::list<Topo_Edge*> m_edges;
    std::list<Topo_Edge*>::iterator m_edges_it;
    Geom_Plane m_plane;

public:
    Topo_Face();
    Topo_Face(const ICanAssociate *);

    void Add(Topo_Edge *edge);
    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;

	virtual double Area();
	
	Geom_Plane GetPlane() const;
	

    void GetFirstWire(Topo_Wire **wire, EnumWireOrder *order);
    void GetNextWire(Topo_Wire **wire, EnumWireOrder *order);

    Topo_Edge* GetFirstEdge();
    Topo_Edge* GetNextEdge();
};

#endif