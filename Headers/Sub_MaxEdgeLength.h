//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Sub_MaxEdgeLength.h - Author Jon Pry

#ifndef SUB_MAXEDGELENGTH_H
#define SUB_MAXEDGELENGTH_H

void MaxEdgeLengthVertexAbsorber(const Geom_Vec3 &pnt, const Geom_Vec3& norm);
void SetupMaxEdgeLength(double length, void (*pRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm), double (*Metric)(const Geom_Vec3& a, const Geom_Vec3& b), Geom_Vec3 (*Subdivide)(const Geom_Vec3 &a, const Geom_Vec3 &b));

#endif
