//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.h - Author Jon Pry 

#ifndef TOPO_FACE_SPHERIC_H
#define TOPO_FACE_SPHERIC_H

#include "Topo_Face.h"
#include "Geom_Vec3.h"
#include "ICanSubdivide.h"

#include <vector>

class Topo_Face_Spheric: public Topo_Face, public ICanSubdivide
{
    double m_radius;
    Geom_Ax2 m_axis;
    mutable double m_metric;
    
    std::vector<Geom_Vec3> m_edge_vertices;

public:
	Topo_Face_Spheric();
    Topo_Face_Spheric(const ICanAssociate *);
    Topo_Face_Spheric(Geom_Ax2 plane,double r);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    void ProjectPoint(const Geom_Vec3 &pnt, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const;
    Geom_Vec3 ParameterizePoint(Geom_Vec3 p, Geom_Vec3 prev) const;
    
//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;

//Overrides from ICanSubdivide
	double MeterDivision(Geom_Vec3 a, Geom_Vec3 b) const;
	Geom_Vec3 Subdivide(Geom_Vec3 a, Geom_Vec3 b) const;
	
	double GetRadius() const;
    
protected:
	void TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvec, Geom_Vec3 norm) const;
	
private:
};

#endif