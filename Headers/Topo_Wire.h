//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Wire.h - Author Jon Pry 

#ifndef TOPO_WIRE_H
#define TOPO_WIRE_H

#include "Topo_Shape.h"
#include "EnumWireOrder.h"
#include "Geom_Vec3.h"
#include "Geom_Plane.h"
#include "ICanCopy.h"
#include "ICanAssociate.h"
#include "ICanPrettyPrint.h"

class Intersection
{
public:
	bool m_exists;
	Geom_Vec3 m_pnt;
	double m_au;
	double m_bu;
	
	Intersection(bool exists, Geom_Vec3 pnt, double au, double bu){m_exists = exists; m_pnt = pnt; m_au = au; m_bu = bu;}
};

class Topo_Wire: public Topo_Shape, public ICanCopyWithTranslation, public ICanAssociate, public ICanPrettyPrint
{
public:
    Topo_Wire();

    virtual void GetVertices(double dDeviation, void (*)(const Geom_Vec3 &pt, const Geom_Vec3 &derivitive)) const = 0;
    virtual Geom_Vec3 GetStart() const = 0;
    virtual Geom_Vec3 GetEnd() const = 0;
    virtual bool HasNorm() const = 0;
    virtual bool IsCoplanar(Geom_Plane &plane) const = 0;
    virtual Geom_Vec3 Norm() const = 0;
    virtual double GetArea(EnumWireOrder order) const = 0;
	virtual Topo_Wire* Project(Geom_Plane &plane) const = 0;
	virtual Intersection Intersect(Topo_Wire *wire) const = 0;
};

#endif

