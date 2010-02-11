//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Geom_Ax2.cpp - Author Jon Pry 

#include "Geom_Ax2.h"
#include "Geom_Plane.h"

Geom_Ax2 Geom_Ax2::Project(Geom_Plane &plane) const
{
	Geom_Vec3 loc = plane.MapPoint(m_loc); 
	return Geom_Ax2(loc,(plane.MapPoint(m_zdir+m_loc)-loc).Normalized(),(plane.MapPoint(m_xdir+m_loc) - loc).Normalized());
}
