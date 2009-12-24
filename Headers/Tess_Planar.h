//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Tess_Planar.h - Author Jon Pry 

#ifndef TESS_PLANAR_H
#define TESS_PLANAR_H

int triangulate_polygon(int, int *, double (*)[2], int (*)[3]);
int is_point_inside_polygon(double *);

#endif 