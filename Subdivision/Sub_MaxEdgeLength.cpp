//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Sub_MaxEdgeLength.cpp - Author Jon Pry
#include "Geom_Vec3.h"

double dMaxEdgeLength;
void (*pMaxEdgeLengthRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm);

Geom_Vec3 vMaxEdgeLengthVerts[3];
int nMaxEdgeLengthV=0;

void CheckTriangle(Geom_Vec3 verts[3])
{
	Geom_Vec3 mvec[3];
	if((verts[1] - verts[0]).Norm() > dMaxEdgeLength)
	{
		Geom_Vec3 mpt = ((verts[1] - verts[0]) / 2) + verts[0];
		mvec[0] = verts[2];
		mvec[1] = verts[0];
		mvec[2] = mpt;
		CheckTriangle(mvec);
		mvec[0] = verts[2];
		mvec[1] = mpt;
		mvec[2] = verts[1];
		CheckTriangle(mvec);
	}
	else if((verts[2] - verts[1]).Norm() > dMaxEdgeLength)
	{
		Geom_Vec3 mpt = ((verts[2] - verts[1]) / 2) + verts[1];
		mvec[0] = verts[0];
		mvec[1] = verts[1];
		mvec[2] = mpt;
		CheckTriangle(mvec);
		mvec[0] = verts[0];
		mvec[1] = mpt;
		mvec[2] = verts[2];
		CheckTriangle(mvec);
	}
	else if((verts[0] - verts[2]).Norm() > dMaxEdgeLength)
	{
		Geom_Vec3 mpt = ((verts[0] - verts[2]) / 2) + verts[2];
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

void SetupMaxEdgeLength(double length, void (*pRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm))
{
	dMaxEdgeLength = length;
	pMaxEdgeLengthRet = pRet;
	nMaxEdgeLengthV = 0;	
}