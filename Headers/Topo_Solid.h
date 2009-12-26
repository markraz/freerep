//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Solid.h - Author Jon Pry 

#ifndef TOPO_SOLID_H
#define TOPO_SOLID_H

#include "Topo_Shape.h"
#include "Topo_Face.h"

class Topo_Solid: public Topo_Shape
{
    std::list<Topo_Face*> m_faces;
    std::list<Topo_Face*>::iterator m_faces_it;

public:
    void Add(Topo_Face *face);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm));
    Topo_Face *GetFirstFace();
    Topo_Face *GetNextFace();
};

#endif