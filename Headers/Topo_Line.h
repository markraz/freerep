//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Line.h - Author Jon Pry 

#ifndef TOPO_LINE_H
#define TOPO_LINE_H

#include "Topo_Wire.h"
#include "Geom_Vec3.h"

class Topo_Line: public Topo_Wire
{
    Geom_Vec3 m_A,m_B;
public:
    Topo_Line(const Geom_Vec3& A, const Geom_Vec3& B);
    ~Topo_Line();
    
    Geom_Line GetLine();
    double GetLength() const;

//Overrides from Topo_Wire
    void GetVertices(double dDeviation, void (*)(const Geom_Vec3 &pt, const Geom_Vec3 &derivitive)) const;
    Geom_Vec3 GetStart() const;
    Geom_Vec3 GetEnd() const;
    bool HasNorm() const;
    Geom_Vec3 Norm() const;
    double GetArea(EnumWireOrder order) const;
    Topo_Wire* Project(Geom_Plane &plane) const;
    bool IsCoplanar(Geom_Plane &plane) const;
    Intersection Intersect(Topo_Wire *wire) const;
    bool IsCoincident(Topo_Wire *wire) const;
    Topo_Wire* Merge(Topo_Wire *wire) const;
    
//Override from ICanPrettyPrint
	void Print();

//Overrides from ICanCopyWithTranslation
    void* MakeTranslatedCopy(Geom_Vec3 dir) const;
};

#endif