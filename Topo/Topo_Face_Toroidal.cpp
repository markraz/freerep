//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Toroidal.cpp - Author Jon Pry 

#include "Topo_Face_Toroidal.h"
#include "Geom_Vec3.h"
#include "math.h"

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

void Topo_Face_Toroidal::TriangulateMinr(double dDeviation, double start, double end, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	//TODO: figure out number of divisions from dDeviation
	
	int ndivisions = 15;
	
	for(int i=0; i < ndivisions; i++)
	{
		double ang1 = i * 2 * M_PI / ndivisions;
		double ang2 = (i+1) * 2 * M_PI / ndivisions;
		double length1 = m_majr + cos(ang1) * m_minr;
		double length2 = m_majr + cos(ang2) * m_minr;
		double height1 = sin(ang1) * m_minr;
		double height2 = sin(ang2) * m_minr;
		
		Geom_Vec3 pnt1(cos(start) * length1, sin(start) * length1, height1);
		Geom_Vec3 norm1(cos(start)*cos(ang1), sin(start)*cos(ang1), sin(ang1));
		Geom_Vec3 pnt2(cos(end) * length1, sin(end) * length1, height1);
		Geom_Vec3 norm2(cos(end)*cos(ang1), sin(end)*cos(ang1), sin(ang1));
		Geom_Vec3 pnt3(cos(start) * length2, sin(start) * length2, height2);
		Geom_Vec3 norm3(cos(start)*cos(ang2), sin(start)*cos(ang2), sin(ang2));
		Geom_Vec3 pnt4(cos(end) * length2, sin(end) * length2, height2);
		Geom_Vec3 norm4(cos(end)*cos(ang2), sin(end)*cos(ang2), sin(ang2));
		
        pRet(pnt1,norm1);
        pRet(pnt2,norm2);
        pRet(pnt3,norm3);

        pRet(pnt4,norm4);
        pRet(pnt2,norm2);
        pRet(pnt3,norm3); 
		
	}
}

void Topo_Face_Toroidal::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	//TODO: figure out number of divisions from dDeviation
	
	int ndivisions = 40;
	
	//TODO: assumes minr < majr/2, different code required elsewise
	
	for(int i=0; i < ndivisions; i++)
	{
		TriangulateMinr(dDeviation, i * 2 * M_PI / ndivisions, (i+1) * 2 * M_PI / ndivisions,pRet);	
	}
}

void *Topo_Face_Toroidal::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Toroidal *nface = new Topo_Face_Toroidal(this);

	nface->m_A.SetLocation(nface->m_A.Location() + dir);
    return nface;
}
