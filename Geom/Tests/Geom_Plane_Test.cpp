//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Plane_Test.cpp - Author Jon Pry 

#include "Geom_Plane.h"
#include "Util_Test.h"

bool Geom_Plane_Test_Map()
{
	Geom_Vec3 norm(0.577350269,0.577350269,0.577350269);
	Geom_Plane plane(norm,norm);
	
	Geom_Vec3 pnt = plane.MapPoint(norm);
	//TODO: check that m_x and m_y are near 0
	
	Geom_Vec3 undone = plane.UnmapPoint(pnt);
	return pnt == undone;	
}

bool Geom_Plane_Test()
{
	return Util_Test_Run(Geom_Plane_Test_Map,"Geom_Plane_Map");
}

