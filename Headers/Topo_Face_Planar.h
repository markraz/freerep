//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Planar.h - Author Jon Pry 

#ifndef TOPO_FACE_PLANAR_H
#define TOPO_FACE_PLANAR_H

#include "Topo_Face.h"

class Topo_Face_Planar: public Topo_Face
{
public:
	Topo_Face_Planar(Geom_Plane plane);
    Topo_Face_Planar(const ICanAssociate *);

//Overrides from Topo_Face
	bool Contains(Topo_Wire* wire);
	double Area();
	
//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
};

#endif 
