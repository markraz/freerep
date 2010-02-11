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

class Topo_Wire: public Topo_Shape, public ICanCopyWithTranslation, public ICanAssociate
{
public:
    Topo_Wire();

    virtual void GetVertices(double dDeviation, void (*)(const Geom_Vec3 &pt, double u)) const = 0;
    virtual Geom_Vec3 GetStart() const = 0;
    virtual Geom_Vec3 GetEnd() const = 0;
    virtual bool HasNorm() const = 0;
    virtual Geom_Vec3 Norm() const = 0;
    virtual double GetArea(EnumWireOrder order) const {return 0;}
	virtual Topo_Wire* Project(Geom_Plane &plane) const = 0;
};

#endif

