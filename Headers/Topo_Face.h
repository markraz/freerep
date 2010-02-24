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
#include "ICanPrettyPrint.h"
#include "ICanTriangulate.h"
#include "Topo_Wire.h"


#include <list>
#include <vector>

enum Topo_Face_Type
{
	TopoFaceConic,
    TopoFacePlanar
};

enum EdgeOrientation{
	EdgeOutside,
	EdgeInside
};

class Topo_Edge;

class OrientedEdge
{
public:
	Topo_Edge *m_edge;
	EdgeOrientation m_orientation;
	
	OrientedEdge(Topo_Edge *edge, EdgeOrientation orientation){m_edge=edge;m_orientation=orientation;}
};

typedef std::pair<Geom_Vec3,Geom_Vec3> NormalizedVertex;

class Topo_Face: public Topo_Shape, public ICanCopyWithTranslation, public ICanAssociate, public ICanTriangulate, public ICanPrettyPrint
{
protected:
    Topo_Face_Type m_type;
    std::list<OrientedEdge> m_edges;
    std::list<OrientedEdge>::iterator m_edges_it;
    Geom_Plane m_plane;

public:
    Topo_Face();
    Topo_Face(const ICanAssociate *);

    virtual void Add(Topo_Edge *edge,bool inside);
    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    
    virtual std::pair<Geom_Vec3,Geom_Vec3> ProjectPoint(Geom_Vec3,Geom_Vec3) const;

	virtual double Area();
	virtual Topo_Wire* Project(Topo_Wire *wire);
	virtual bool Contains(Topo_Wire* wire);
	virtual Geom_Vec3 ParameterizePoint(Geom_Vec3 p, Geom_Vec3 derivitive) const;
	
	Geom_Plane GetPlane() const;
	

    void GetFirstWire(Topo_Wire **wire, EnumWireOrder *order);
    void GetNextWire(Topo_Wire **wire, EnumWireOrder *order);

    Topo_Edge* GetFirstEdge();
    Topo_Edge* GetNextEdge();
    
protected:
	virtual void TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvec, Geom_Vec3 norm) const;
};

#endif