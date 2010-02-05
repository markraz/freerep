//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Sub_MaxEdgeLength.cpp - Author Jon Pry
#include "Geom_Vec3.h"
#include "ICanSubdivide.h"

void (*pMaxEdgeLengthRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm);
const ICanSubdivide *pSubdivide;

Geom_Vec3 vMaxEdgeLengthVerts[3];
int nMaxEdgeLengthV=0;

void CheckTriangle(Geom_Vec3 verts[3])
{
	Geom_Vec3 mvec[3];
	double norm0 = pSubdivide->MeterDivision(verts[1],verts[0]);
	double norm1 = pSubdivide->MeterDivision(verts[2],verts[1]);
	double norm2 = pSubdivide->MeterDivision(verts[0],verts[2]);
	bool doedge0 = false;
	bool doedge1 = false;
	bool doedge2 = false;
	
	if(norm0 > 1 && norm0 >= norm1 && norm0 >= norm2)
		doedge0=true;
	else if(norm1 > 1 && norm1 >= norm0 && norm1 >= norm2)
		doedge1=true;	
	else if(norm2 > 1 && norm2 >= norm0 && norm2 >= norm1)
		doedge2=true;
	
		
	if(doedge0)
	{
		Geom_Vec3 mpt = pSubdivide->Subdivide(verts[1],verts[0]);
		mvec[0] = verts[2];
		mvec[1] = verts[0];
		mvec[2] = mpt;
		CheckTriangle(mvec);
		mvec[0] = verts[2];
		mvec[1] = mpt;
		mvec[2] = verts[1];
		CheckTriangle(mvec);
	}
	else if(doedge1)
	{
		Geom_Vec3 mpt = pSubdivide->Subdivide(verts[2],verts[1]);
		mvec[0] = verts[0];
		mvec[1] = verts[1];
		mvec[2] = mpt;
		CheckTriangle(mvec);
		mvec[0] = verts[0];
		mvec[1] = mpt;
		mvec[2] = verts[2];
		CheckTriangle(mvec);
	}
	else if(doedge2)
	{
		Geom_Vec3 mpt = pSubdivide->Subdivide(verts[0],verts[2]);
		mvec[0] = verts[1];
		mvec[1] = verts[2];
		mvec[2] = mpt;
		CheckTriangle(mvec);
		mvec[0] = verts[1];
		mvec[1] = mpt;
		mvec[2] = verts[0];
		CheckTriangle(mvec);
	}
	else
	{
		Geom_Vec3 norm(0,0,1);
		pMaxEdgeLengthRet(verts[0],norm);
		pMaxEdgeLengthRet(verts[1],norm);
		pMaxEdgeLengthRet(verts[2],norm);
	}
}

void MaxEdgeLengthVertexAbsorber(const Geom_Vec3 &pnt, const Geom_Vec3 &norm)
{
	vMaxEdgeLengthVerts[nMaxEdgeLengthV++] = pnt;
	if(nMaxEdgeLengthV > 2)
	{
		nMaxEdgeLengthV = 0;
		CheckTriangle(vMaxEdgeLengthVerts);
	}
} 

void SetupMaxEdgeLength(void (*pRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm), const ICanSubdivide *Subdivide)
{
	pMaxEdgeLengthRet = pRet;
	pSubdivide = Subdivide;
	nMaxEdgeLengthV = 0;	
}