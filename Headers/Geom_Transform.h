//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Transform.h - Author Jon Pry 

#ifndef GEOM_TRANSFORM
#define GEOM_TRANSFORM

#include "Geom_Matrix.h"

class Geom_Transform
{
	Geom_Matrix m_mat;
	Geom_Vec3 m_vec;	
	
public:
	Geom_Transform(Geom_Matrix mat, Geom_Vec3 vec);
	
	Geom_Vec3 Transform(Geom_Vec3);
	Geom_Vec3 Rotate(Geom_Vec3);
	Geom_Matrix GetMatrix();
	Geom_Vec3 GetTranslation();
};

#endif
