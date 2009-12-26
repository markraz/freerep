//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Toroidal.h - Author Jon Pry 

#ifndef TOPO_FACE_TOROIDAL_H
#define TOPO_FACE_TOROIDAL_H

#include "Topo_Face.h"
#include "Geom_Ax2.h"

class Topo_Face_Toroidal: public Topo_Face
{
    Geom_Ax2 m_A;
    double m_majr;
    double m_minr;

public:
	Topo_Face_Toroidal();
    Topo_Face_Toroidal(const ICanAssociate *);
    Topo_Face_Toroidal(Geom_Ax2 A, double majr, double minr);

    void Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt,const Geom_Vec3&norm));
    
 //Override from ICanCopyAndTranslate
	void *MakeTranslatedCopy(Geom_Vec3 dir) const;
	
private:
	void TriangulateMinr(double dDeviation, double start, double end,void (*pRet)(const Geom_Vec3&pnt,const Geom_Vec3&norm));
};

#endif