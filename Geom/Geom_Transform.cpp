//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Transform.cpp - Author Jon Pry

#include "Geom_Transform.h"

Geom_Transform::Geom_Transform(Geom_Matrix mat, Geom_Vec3 vec)
{
	m_mat = mat;
	m_vec = vec;	
} 

Geom_Vec3 Geom_Transform::Transform(Geom_Vec3 pnt)
{
	return m_mat.Multiply(pnt) + m_vec;	
}

Geom_Vec3 Geom_Transform::Rotate(Geom_Vec3 pnt)
{
	return m_mat.Multiply(pnt);	
}