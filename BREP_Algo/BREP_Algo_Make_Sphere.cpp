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
	Topo_Arc *a1 = MakeArcForSphere(loc,r,loc.XDir(),loc.ZDir(),-M_PI,0);	
	Topo_Arc *a2 = MakeArcForSphere(loc,r,loc.XDir(),loc.ZDir(),0,M_PI);
	
	Topo_Edge *edge1 = new Topo_Edge();
	edge1->Add(a1,BFirst);
	edge1->Add(a2);
	
	Topo_Edge *edge2 = new Topo_Edge();
	edge2->Add(a1,AFirst);
	edge2->Add(a2);
	
	Topo_Face *face1 = new Topo_Face_Spheric(loc,r);
	face1->Add(edge1);

	Topo_Face *face2 = new Topo_Face_Spheric(Geom_Ax2(loc.Location(),loc.ZDir(),loc.XDir()*-1),r);
	face2->Add(edge2);

	Topo_Solid* solid = new Topo_Solid();
	solid->Add(face1);
	solid->Add(face2);
	
	return solid;
}