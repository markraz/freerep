//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Area.ch - Author Jon Pry

#ifndef BREP_ALGO_INSIDE_H
#define BREP_ALGO_INSIDE_H

#include "Geom_Vec3.h"

#include <vector>

bool Inside(Geom_Vec3 pnt, std::vector<Geom_Vec3> &poly);

#endif 