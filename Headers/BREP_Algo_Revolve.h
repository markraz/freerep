//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Revolve.h - Author Jon Pry 

#ifndef BREP_ALGO_REVOLVE_H
#define BREP_ALGO_REVOLVE_H

#include "Topo_Face_Compound.h"
#include "Topo_Line.h"

Topo_Face_Compound* RevolveSkeleton(Topo_Line *line, Topo_Shape *generatrix);

#endif
