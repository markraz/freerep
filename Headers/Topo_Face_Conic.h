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
    Topo_Face_Conic(Geom_Ax2 axis, double r1, double r2, double length);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    Geom_Vec3 ParameterizePoint(Geom_Vec3 p, Geom_Vec3 derivitive) const;
    std::pair<Geom_Vec3,Geom_Vec3> ProjectPoint(Geom_Vec3,Geom_Vec3) const;
    bool Contains(Topo_Wire* wire);
    
//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
//Overrides from ICanSubdivide
	double MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const;
	Geom_Vec3 Subdivide(Geom_Vec3 a, Geom_Vec3 b) const;
	
	double GetRadius1() const;
	double GetRadius2() const;
	double GetLength() const;
	
protected:
	void TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvec, Geom_Vec3 norm) const;
	    
private:
};

#endif