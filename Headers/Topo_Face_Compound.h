//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Compound.h - Author Jon Pry 

#ifndef TOPO_FACE_COMPOUND_H
#define TOPO_FACE_COMPOUND_H

#include "Topo_Face.h"
#include "Topo_Line.h"

#include <vector>

class Topo_Face_Compound: public Topo_Face
{
	Topo_Line *m_directrix;
	std::vector<Topo_Face*> m_faces;
	
public:
	Topo_Face_Compound(Topo_Line *directrix);
	
	void Add(Topo_Face *face);
	Topo_Face* GetFaces();
	
//Overrides from Topo_Face
	void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const;
    void Add(Topo_Edge *edge,bool inside);

//Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
};

#endif 
