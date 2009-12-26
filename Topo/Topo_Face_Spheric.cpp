//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Spheric.cpp - Author Jon Pry 

#include "Topo_Face_Spheric.h"
#include "Geom_Vec3.h"

#define tao 1.61803399

Geom_Vec3 icosahedron_verts[] = {
	Geom_Vec3(1,tao,0),Geom_Vec3(-1,tao,0),Geom_Vec3(1,-tao,0),
	Geom_Vec3(-1,-tao,0),Geom_Vec3(0,1,tao),Geom_Vec3(0,-1,tao),
	Geom_Vec3(0,1,-tao),Geom_Vec3(0,-1,-tao),Geom_Vec3(tao,0,1),
	Geom_Vec3(-tao,0,1),Geom_Vec3(tao,0,-1),Geom_Vec3(-tao,0,-1)};

int icosahedron_faces[][3] = {
	{0,1,4},{1,9,4},{4,9,5},
	{5,9,3},{2,3,7},{3,2,5},
	{7,10,2},{0,8,10},{0,4,8},
	{8,2,10},{8,4,5},{8,5,2},
	{1,0,6},{11,1,6},{3,9,11},
	{6,10,7},{3,11,7},{11,6,7},
	{6,0,10},{9,1,11}};
	
Topo_Face_Spheric::Topo_Face_Spheric()
{
	
}

Topo_Face_Spheric::Topo_Face_Spheric(Geom_Vec3 c, double radius)
{
	m_C = c; m_radius = radius;	
}

Topo_Face_Spheric::Topo_Face_Spheric(const ICanAssociate *associate):Topo_Face(associate)
{
	
}

void Topo_Face_Spheric::ProjectPoint(Geom_Vec3 &pnt, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	pRet(pnt.Normalized() * m_radius + m_C,pnt.Normalized());
}

void Topo_Face_Spheric::OutputTri(Geom_Vec3 &pnt1, Geom_Vec3 &pnt2, Geom_Vec3 &pnt3, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	ProjectPoint(pnt1,pRet);
	ProjectPoint(pnt2,pRet);
	ProjectPoint(pnt3,pRet);
}

void Topo_Face_Spheric::SplitFace(Geom_Vec3 &pnt1, Geom_Vec3 &pnt2, Geom_Vec3 &pnt3, int ndivisions, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	if(ndivisions == 1)
	{
		OutputTri(pnt1,pnt2,pnt3,pRet);
		return;
	}
		
	Geom_Vec3 npnt2 = ((pnt1 - pnt2) / ndivisions) + pnt2;
	Geom_Vec3 npnt3 = ((pnt1 - pnt3) / ndivisions) + pnt3;
	
	Geom_Vec3 vec = (npnt3 - npnt2) / (ndivisions - 1);
	
	Geom_Vec3 cbpnt = pnt2;
	Geom_Vec3 ctpnt = npnt2;
	
	for(int i=0; i < ndivisions-1;i++)
	{
		Geom_Vec3 ncbpnt = cbpnt+vec;
		Geom_Vec3 nctpnt = ctpnt+vec;
		OutputTri(cbpnt,ctpnt,ncbpnt,pRet);
		OutputTri(ctpnt,nctpnt,ncbpnt,pRet);
		cbpnt = ncbpnt;
		ctpnt = nctpnt;
	} 
	
	OutputTri(ctpnt,pnt3,cbpnt,pRet);
	
	SplitFace(pnt1,npnt2,npnt3, ndivisions - 1, pRet);
}

void Topo_Face_Spheric::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm))
{
	//TODO: figure out number of divisions from dDeviation
	
	int ndivisions = 7;
	for(int i=0; i < 20; i++)
	{
		SplitFace(icosahedron_verts[icosahedron_faces[i][0]],icosahedron_verts[icosahedron_faces[i][1]],icosahedron_verts[icosahedron_faces[i][2]],ndivisions,pRet);	
	}
}

void *Topo_Face_Spheric::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face_Spheric *nface = new Topo_Face_Spheric(this);

	nface->m_C = nface->m_C + dir;
    return nface;
}
