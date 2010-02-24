//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Make_Cone.cpp - Author Jon Pry

#include "BREP_Algo_Make_Cone.h"
#include "Topo_Arc.h"
#include "Topo_Line.h"
#include "Topo_Face.h"
#include "Topo_Face_Conic.h"
#include "Topo_Solid.h"
#include "Geom_Matrix.h"
#include "FreeREP.h"

#include <math.h>

Topo_Face* MakeConeSectionSkeleton(Geom_Ax2 loc, double r1, double r2, double length, double sa)
{
	Geom_Matrix m = Geom_Matrix::RotateAround(loc.ZDir(),sa-M_PI);
	return new Topo_Face_Conic(Geom_Ax2(loc.Location(),loc.ZDir(),m.Multiply(loc.XDir())),r1,r2,length);
}

Topo_Arc* MakeArcForCone(Geom_Ax2 loc, double r, double start, double end)
{
	//Todo, transform the specified z and x vector by the Geom_Ax2 vector	
	return new Topo_Arc(loc,r,start,end);
}


Topo_Shape * MakeCone(Geom_Ax2 loc, double r1, double r2, double length)
{
	Geom_Vec3 p1 = loc.Location() - loc.ZDir() * (length/2);
	Geom_Vec3 p2 = loc.Location() + loc.ZDir() * (length/2);
	
	Geom_Ax2 ax1 = Geom_Ax2(p1, loc.ZDir()*-1, loc.XDir()*-1);	
	Geom_Ax2 ax2 = Geom_Ax2(p2, loc.ZDir(), loc.XDir());
	
	Topo_Arc *a1 = MakeArcForCone(Geom_Ax2(p1,loc.ZDir(),loc.XDir()),r1,-M_PI/2,M_PI/2);
	Topo_Arc *a2 = MakeArcForCone(Geom_Ax2(p1,loc.ZDir(),loc.XDir()),r1,M_PI/2,3*M_PI/2);
	
	Topo_Arc *a3 = MakeArcForCone(Geom_Ax2(p2,loc.ZDir(),loc.XDir()),r2,-M_PI/2,M_PI/2);
	Topo_Arc *a4 = MakeArcForCone(Geom_Ax2(p2,loc.ZDir(),loc.XDir()),r2,M_PI/2,3*M_PI/2);
	
	Topo_Line *l1 = new Topo_Line(a1->GetStart(),a3->GetStart());
	Topo_Line *l2 = new Topo_Line(a3->GetEnd(),a1->GetEnd());
	
	
	Topo_Edge *edge1 = new Topo_Edge();
	edge1->Add(a1,BFirst);
	edge1->Add(l1);
	edge1->Add(a3);
	edge1->Add(l2);
	
	Topo_Edge *edge2 = new Topo_Edge();
	edge2->Add(a2,BFirst);
	edge2->Add(l1);
	edge2->Add(a4);
	edge2->Add(l2);
	
	Topo_Face* face1 = new Topo_Face_Conic(loc,r1,r2,length);
	face1->Add(edge1,false);
	
	//TODO: reverse the x-axis? on this face
	Topo_Face* face2 = new Topo_Face_Conic(Geom_Ax2(loc.Location(),loc.ZDir(),loc.XDir() * -1),r1,r2,length);
	face2->Add(edge2,false);
	
	Topo_Solid *solid = new Topo_Solid();
	solid->Add(face1);
	solid->Add(face2);
	return solid;
}

