//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//ICanTriangulate.h - Author Jon Pry 

#ifndef ICANTRIANGULATE_H
#define ICANTRIANGULATE_H

class ICanTriangulate
{
public:
	virtual void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm)) const = 0;
};

#endif 
