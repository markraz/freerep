//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Toroidal.h"
#include "Geom_Vec3.h"

Topo_Face_Toroidal::Topo_Face_Toroidal()
{
	
}

Topo_Face_Toroidal::Topo_Face_Toroidal(Geom_Ax2 A, double majr, double minr)
{
	m_A = A; m_majr = majr; m_minr = minr;	
}

Topo_Face_Toroidal::Topo_Face_Toroidal(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

void Topo_Face_Toroidal::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	//TODO: figure out number of divisions from dDeviation
	
	int ndivisions = 7;
}

void *Topo_Face_Toroidal::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Toroidal *nface = new Topo_Face_Toroidal(this);

	nface->m_A.SetLocation(nface->m_A.Location() + dir);
    return nface;
}
