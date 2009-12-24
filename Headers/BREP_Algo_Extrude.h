//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Extrude.cpp - Author Jon Pry 


#ifndef BREP_ALGO_EXTRUDE_H
#define BREP_ALGO_EXTRUDE_H

#include "Topo_Face.h"
#include "Geom_Vec3.h"
#include "Topo_Solid.h"

Topo_Solid* BrepAlgoExtrude(Topo_Face *face, Geom_Vec3 dir);

#endif