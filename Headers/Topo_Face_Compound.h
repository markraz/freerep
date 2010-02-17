//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Compound.h - Author Jon Pry 

#ifndef TOPO_FACE_COMPOUND_H
#define TOPO_FACE_COMPOUND_H

#include "Topo_Face.h"

#include <vector>

class Topo_Face_Compound: public Topo_Face
{
	std::vector<Topo_Face*> m_faces;
	
public:
	Topo_Face_Compound();
	
	void Add(Topo_Face *face);
	
//Overrides from Topo_Face
	void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    void Add(Topo_Edge *edge);
    void Add(Topo_Edge *edge,bool inside);

//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
};

#endif 
