//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Sub_MaxEdgeLength.h - Author Jon Pry

#ifndef SUB_MAXEDGELENGTH_H
#define SUB_MAXEDGELENGTH_H

#include "ICanSubdivide.h"

void MaxEdgeLengthVertexAbsorber(const Geom_Vec3 &pnt, const Geom_Vec3& norm);
void SetupMaxEdgeLength(void (*pRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm), const ICanSubdivide *subdivide);

#endif
