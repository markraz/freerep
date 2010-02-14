//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Compound.cpp - Author Jon Pry

#include "Topo_Face_Compound.h"

Topo_Face_Compound::Topo_Face_Compound()
{
	
}

void Topo_Face_Compound::Add(Topo_Face *face)
{
	m_faces.push_back(face);	
} 

void *Topo_Face_Compound::MakeTranslatedCopy(Geom_Vec3 dir) const
{
	
}