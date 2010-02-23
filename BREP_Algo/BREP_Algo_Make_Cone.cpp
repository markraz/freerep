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

Topo_Face * CreateConeFace(Geom_Ax2 ax, double r1, double r2, double length, double t1, double t2)
{
	Geom_Matrix m = Geom_Matrix::RotateAround(ax.ZDir(),(t1+t2)/2);
	
	Geom_Ax2 coneax(ax.Location(),m.Multiply(ax.YDir()),m.Multiply(ax.ZDir()));
	
	Geom_Vec3 x = coneax.XDir();
	Geom_Vec3 y = coneax.YDir();
	Geom_Vec3 z = coneax.ZDir();
		
	Topo_Face *f = new Topo_Face_Conic(coneax, ax, r1, r2, length);
	return f;
}


std::vector<Topo_Face*> MakeConeSectionSkeleton(Geom_Ax2 loc, double r1, double r2, double length, double sa, double ea)
{
	std::vector<Topo_Face*> ret;
	
	//TODO: implement me
	for(double a = sa; a < ea && !ISZERO(a-ea); a +=M_PI/2)
	{
		ret.push_back(CreateConeFace(loc,r1,r2,length,a,a+M_PI/2));
	}
	
	return ret;
}

Topo_Face * CreateConeFace(Topo_Wire* s1, Topo_Wire* s2, Topo_Wire *s3, Topo_Wire *s4, Geom_Ax2 ax, double r1, double r2, double length, double t1, double t2)
{
	Topo_Edge *e = new Topo_Edge();
	e->Add(s1);//,BFirst);
	e->Add(s2);
	e->Add(s3);
	e->Add(s4);
	
	Topo_Face *f = CreateConeFace(ax, r1, r2, length,t1,t2);
	f->Add(e);
	return f;
}

Topo_Shape * MakeCone(Geom_Ax2 loc, double r1, double r2, double length)
{
	Geom_Vec3 p1 = loc.Location() - loc.ZDir() * (length/2);
	Geom_Vec3 p2 = loc.Location() + loc.ZDir() * (length/2);
	
	Geom_Ax2 ax1 = Geom_Ax2(p1, loc.ZDir()*-1, loc.XDir()*-1);	
	Geom_Ax2 ax2 = Geom_Ax2(p2, loc.ZDir(), loc.XDir());
	
	Topo_Arc *a1 = new Topo_Arc(ax1,r1,0,M_PI/2);
	Topo_Arc *a2 = new Topo_Arc(ax1,r1,M_PI/2,M_PI);
	Topo_Arc *a3 = new Topo_Arc(ax1,r1,M_PI,3*M_PI/2);
	Topo_Arc *a4 = new Topo_Arc(ax1,r1,3*M_PI/2,2*M_PI);
	
	Topo_Arc *a5 = new Topo_Arc(ax2,r2,0,-M_PI/2);
	Topo_Arc *a6 = new Topo_Arc(ax2,r2,-M_PI/2,-M_PI);
	Topo_Arc *a7 = new Topo_Arc(ax2,r2,-M_PI,-3*M_PI/2);
	Topo_Arc *a8 = new Topo_Arc(ax2,r2,-3*M_PI/2,-2*M_PI);
	
	
	Geom_Vec3 a1start = a1->GetStart();
	Geom_Vec3 a1end = a1->GetEnd();
	Geom_Vec3 a2start = a2->GetStart();
	Geom_Vec3 a2end = a2->GetEnd();
	Geom_Vec3 a3start = a3->GetStart();
	Geom_Vec3 a3end = a3->GetEnd();
	Geom_Vec3 a4start = a4->GetStart();
	Geom_Vec3 a4end = a4->GetEnd();
	
	Geom_Vec3 a5start = a5->GetStart();
	Geom_Vec3 a5end = a5->GetEnd();
	Geom_Vec3 a6start = a6->GetStart();
	Geom_Vec3 a6end = a6->GetEnd();
	Geom_Vec3 a7start = a7->GetStart();
	Geom_Vec3 a7end = a7->GetEnd();
	Geom_Vec3 a8start = a8->GetStart();
	Geom_Vec3 a8end = a8->GetEnd();
	
	 
	//TODO: handle zero radius ends
	Topo_Line *l1 = new Topo_Line(a1->GetStart(),a7->GetStart());
	Topo_Line *l2 = new Topo_Line(a1->GetEnd(),a7->GetEnd());
	
	Topo_Line *l3 = l2;//new Topo_Line(a2->GetStart(),a8->GetStart());
	Topo_Line *l4 = new Topo_Line(a2->GetEnd(),a8->GetEnd());

	Topo_Line *l5 = l4;//new Topo_Line(a3->GetStart(),a5->GetStart());
	Topo_Line *l6 = new Topo_Line(a3->GetEnd(),a5->GetEnd());
	
	Topo_Line *l7 = l6;//new Topo_Line(a4->GetStart(),a6->GetStart());
	Topo_Line *l8 = l1;//new Topo_Line(a4->GetEnd(),a6->GetEnd());
	
	Geom_Vec3 l1start = l1->GetStart();
	Geom_Vec3 l1end = l1->GetEnd();
	Geom_Vec3 l2start = l2->GetStart();
	Geom_Vec3 l2end = l2->GetEnd();
	Geom_Vec3 l3start = l3->GetStart(); 
	Geom_Vec3 l3end = l3->GetEnd();
	Geom_Vec3 l4start = l4->GetStart();
	Geom_Vec3 l4end = l4->GetEnd();
	
	Geom_Vec3 l5start = l5->GetStart();
	Geom_Vec3 l5end = l5->GetEnd();
	Geom_Vec3 l6start = l6->GetStart(); 
	Geom_Vec3 l6end = l6->GetEnd();
	Geom_Vec3 l7start = l7->GetStart();
	Geom_Vec3 l7end = l7->GetEnd();
	Geom_Vec3 l8start = l8->GetStart();
	Geom_Vec3 l8end = l8->GetEnd();
	
	Topo_Solid *solid = new Topo_Solid();
	solid->Add(face1);
	solid->Add(face2);
	return solid;
}

