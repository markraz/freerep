//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Arc.h - Author Jon Pry 

#ifndef TOPO_ARC_H
#define TOPO_ARC_H

#include "Topo_Wire.h"
#include "Geom_Ax2.h"

class Topo_Arc: public Topo_Wire
{
    Geom_Ax2 m_A;
    double m_radius,m_start,m_end;
public:
    Topo_Arc(const Geom_Ax2& A, double radius, double start, double end);

    Geom_Vec3 GetPntAtU(double u) const;
    Geom_Ax2 GetAxis() const;

//Overrides from Topo_Wire
    void GetVertices(double dDeviation, void (*)(const Geom_Vec3 &pt, double u)) const;
    Geom_Vec3 GetStart() const;
    Geom_Vec3 GetEnd() const;
    bool HasNorm() const;
    Geom_Vec3 Norm() const;
    double GetArea(EnumWireOrder order) const;
	Topo_Wire* Project(Geom_Plane &plane) const;
	bool IsCoplanar(Geom_Plane &plane) const;

//Override from ICanPrettyPrint
	void Print();

//Overrides from ICanCopyWithTranslation
    void* MakeTranslatedCopy(Geom_Vec3 dir) const;

private:
	double IntegrateArc(double t) const;
};

#endif
