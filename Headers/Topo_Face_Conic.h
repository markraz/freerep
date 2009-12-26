//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Conic.h - Author Jon Pry 

#ifndef TOPO_FACE_CONIC_H
#define TOPO_FACE_CONIC_H

#include "Topo_Face.h"
#include "Topo_Arc.h"

class Topo_Face_Conic: public Topo_Face
{
    Topo_Arc *m_arc1, *m_arc2;

public:
    Topo_Face_Conic(Topo_Arc *arc1, Topo_Arc *arc2);
    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm));
    
 //Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
    
};

#endif