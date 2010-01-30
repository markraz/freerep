//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.h - Author Jon Pry 

#ifndef TOPO_FACE_SPHERIC_H
#define TOPO_FACE_SPHERIC_H

#include "Topo_Face.h"
#include "Geom_Vec3.h"

#include <vector>

class Topo_Face_Spheric: public Topo_Face
{
    double m_radius;
    
    std::vector<Geom_Vec3> m_edge_vertices;

public:
	Topo_Face_Spheric();
    Topo_Face_Spheric(const ICanAssociate *);
    Topo_Face_Spheric(Geom_Plane plane,double r);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    void ProjectPoint(const Geom_Vec3 &pnt, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const;
    
 //Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
	double GetRadius() const;
    
private:
};

#endif