//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//SimpleIntersector.h - Author Jon Pry 

#ifndef SIMPLEINTERSECTOR_H
#define SIMPLEINTERSECTOR_H

#include "Topo_Wire.h"

#include <vector>

class SimpleIntersector
{
public:
	void Intersect(std::vector<Topo_Wire*> &wires);
};

#endif 
