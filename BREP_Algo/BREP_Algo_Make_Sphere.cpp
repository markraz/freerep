//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Make_Sphere.cpp - Author Jon Pry

#include "BREP_Algo_Make_Sphere.h"
#include "Topo_Arc.h"
#include "Topo_Edge.h"
#include "Topo_Face_Spheric.h"
#include "Geom_Plane.h"
#include "Topo_Solid.h"

#include <math.h>

Topo_Arc* MakeArcForSphere(Geom_Ax2 loc, double r, Geom_Vec3 z_dir, Geom_Vec3 x_dir, double start, double end)
{
	//Todo, transform the specified z and x vector by the Geom_Ax2 vector	
	return new Topo_Arc(Geom_Ax2(loc.Location(),z_dir,x_dir),r,start,end);
}

Topo_Shape * MakeSphere(Geom_Ax2 loc, double r)
{
	//Arcs in XY Plane
	Topo_Arc *a1 = MakeArcForSphere(loc,r,Geom_Vec3(0,0,1),Geom_Vec3(1,0,0),0,-M_PI/2);	
	Topo_Arc *a2 = MakeArcForSphere(loc,r,Geom_Vec3(0,0,1),Geom_Vec3(1,0,0),-M_PI/2,-M_PI);
	Topo_Arc *a3 = MakeArcForSphere(loc,r,Geom_Vec3(0,0,1),Geom_Vec3(1,0,0),-M_PI,-M_PI*3/2);
	Topo_Arc *a4 = MakeArcForSphere(loc,r,Geom_Vec3(0,0,1),Geom_Vec3(1,0,0),-M_PI*3/2,-M_PI*2);
	
	//Arcs in the XZ
	Topo_Arc *a5 = MakeArcForSphere(loc,r,Geom_Vec3(0,-1,0),Geom_Vec3(1,0,0),-M_PI/2,0);
	Topo_Arc *a6 = MakeArcForSphere(loc,r,Geom_Vec3(0,-1,0),Geom_Vec3(1,0,0),0,M_PI/2);
	Topo_Arc *a7 = MakeArcForSphere(loc,r,Geom_Vec3(0,-1,0),Geom_Vec3(1,0,0),M_PI/2,M_PI);
	Topo_Arc *a8 = MakeArcForSphere(loc,r,Geom_Vec3(0,-1,0),Geom_Vec3(1,0,0),M_PI,M_PI*3/2);
	
	//Arcs in the Y Z
	Topo_Arc *a9 = MakeArcForSphere(loc,r,Geom_Vec3(-1,0,0),Geom_Vec3(0,-1,0),-M_PI/2,0);
	Topo_Arc *a10 = MakeArcForSphere(loc,r,Geom_Vec3(-1,0,0),Geom_Vec3(0,-1,0),0,M_PI/2);
	Topo_Arc *a11 = MakeArcForSphere(loc,r,Geom_Vec3(-1,0,0),Geom_Vec3(0,-1,0),M_PI/2,M_PI);
	Topo_Arc *a12 = MakeArcForSphere(loc,r,Geom_Vec3(-1,0,0),Geom_Vec3(0,-1,0),M_PI,M_PI*3/2);
	
	Topo_Edge *edge1 = new Topo_Edge();
	edge1->Add(a1);
	edge1->Add(a5);
	edge1->Add(a9);
	
	Geom_Vec3 vec1(0.577350269,-0.577350269,-0.577350269);
	Topo_Face *face1 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec1),r);
	face1->Add(edge1);

/*
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
	
	Geom_Vec3 a9start = a9->GetStart();
	Geom_Vec3 a9end = a9->GetEnd();
	Geom_Vec3 a10start = a10->GetStart();
	Geom_Vec3 a10end = a10->GetEnd();
	Geom_Vec3 a11start = a11->GetStart();
	Geom_Vec3 a11end = a11->GetEnd();
	Geom_Vec3 a12start = a12->GetStart();
	Geom_Vec3 a12end = a12->GetEnd();
*/
	
	Topo_Edge *edge2 = new Topo_Edge();
	edge2->Add(a2);
	edge2->Add(a8);
	edge2->Add(a9);
	
	Geom_Vec3 vec2(-0.577350269,-0.577350269,-0.577350269);
	Topo_Face *face2 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec2),r);
	face2->Add(edge2);
	
	Topo_Edge *edge3 = new Topo_Edge();
	edge3->Add(a3);
	edge3->Add(a8);
	edge3->Add(a12);
	
	Geom_Vec3 vec3(-0.577350269,0.577350269,-0.577350269);
	Topo_Face *face3 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec3),r);
	face3->Add(edge3);
	
	Topo_Edge *edge4 = new Topo_Edge();
	edge4->Add(a4);
	edge4->Add(a5);
	edge4->Add(a12);
	
	Geom_Vec3 vec4(0.577350269,0.577350269,-0.577350269);
	Topo_Face *face4 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec4),r);
	face4->Add(edge4);

	Topo_Edge *edge5 = new Topo_Edge();
	edge5->Add(a6);
	edge5->Add(a1);
	edge5->Add(a10);
	
	Geom_Vec3 vec5(0.577350269,-0.577350269,0.577350269);
	Topo_Face *face5 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec5),r);
	face5->Add(edge5);
	
	Topo_Edge *edge6 = new Topo_Edge();
	edge6->Add(a7);
	edge6->Add(a2);
	edge6->Add(a10);
	
	Geom_Vec3 vec6(-0.577350269,-0.577350269,0.577350269);
	Topo_Face *face6 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec6),r);
	face6->Add(edge6);
	
	Topo_Edge *edge7 = new Topo_Edge();
	edge7->Add(a11);
	edge7->Add(a7);
	edge7->Add(a3);
	
	Geom_Vec3 vec7(-0.577350269,0.577350269,0.577350269);
	Topo_Face *face7 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec7),r);
	face7->Add(edge7);
	
	Topo_Edge *edge8 = new Topo_Edge();
	edge8->Add(a11,BFirst);
	edge8->Add(a4);
	edge8->Add(a6);
	
	Geom_Vec3 vec8(0.577350269,0.577350269,0.577350269);
	Topo_Face *face8 = new Topo_Face_Spheric(Geom_Plane(loc.Location(),vec8),r);
	face8->Add(edge8);

	Topo_Solid* solid = new Topo_Solid();
	solid->Add(face1);
	solid->Add(face2);
	solid->Add(face3);
	solid->Add(face4);
	solid->Add(face5);
	solid->Add(face6);
	solid->Add(face7);
	solid->Add(face8);
	
	return solid;
}