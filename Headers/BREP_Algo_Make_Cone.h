//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Make_Cone.h - Author Jon Pry

#ifndef BREP_ALGO_MAKE_CONE_H
#define BREP_ALGO_MAKE_CONE_H

#include "Topo_Shape.h"
#include "Topo_Face.h"
#include "Geom_Ax2.h"

#include <vector>

Topo_Shape * MakeCone(Geom_Ax2 loc, double r1, double r2, double length);
std::vector<Topo_Face*> MakeConeSectionSkeleton(Geom_Ax2 loc, double r1, double r2, double length, double sa, double ea);

#endif 