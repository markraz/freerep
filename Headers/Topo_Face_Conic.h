//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.h - Author Jon Pry 

#ifndef TOPO_FACE_CONIC_H
#define TOPO_FACE_CONIC_H

#include "Topo_Face.h"
#include "Geom_Vec3.h"
#include "Geom_Ax2.h"
#include "ICanSubdivide.h"

#include <vector>

class Topo_Face_Conic: public Topo_Face, public ICanSubdivide
{
    double m_radius_1;
    double m_radius_2;
    double m_length;
    Geom_Ax2 m_axis;
    
    mutable double m_deviation;
    
    std::vector<Geom_Vec3> m_edge_vertices;

public:
	Topo_Face_Conic();
    Topo_Face_Conic(const ICanAssociate *);
    Topo_Face_Conic(Geom_Ax2 axis, Geom_Ax2 caxis, double r1, double r2, double length);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    void ProjectPoint(const Geom_Vec3 &pnt, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const;
    
//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
//Overrides from ICanSubdivide
	double MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const;
	Geom_Vec3 Subdivide(Geom_Vec3 a, Geom_Vec3 b) const;

	
	double GetRadius1() const;
	double GetRadius2() const;
	double GetLength() const;
    
private:
};

#endif