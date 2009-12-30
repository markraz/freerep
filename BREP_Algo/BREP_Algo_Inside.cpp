//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Area.cpp - Author Jon Pry

#include "BREP_Algo_Inside.h"
#include "Geom_Vec3.h"
#include "FreeREP.h"

#include <vector>

enum EnumCrossingType 
{
	CrossingTypeMaybe,
	CrossingTypeYes,
	CrossingTypeNo	
};

EnumCrossingType Crosses(Geom_Vec3 pnt, Geom_Vec3 start, Geom_Vec3 end)
{
	if(start.m_x < pnt.m_x - FreeREP::CONFUSION && end.m_x < pnt.m_x - FreeREP::CONFUSION)
		return CrossingTypeNo;
		
	if(start.m_y < pnt.m_y - FreeREP::CONFUSION && end.m_y < pnt.m_y - FreeREP::CONFUSION)
		return CrossingTypeNo;
		
	if(start.m_y > pnt.m_y + FreeREP::CONFUSION && end.m_y > pnt.m_y + FreeREP::CONFUSION)
		return CrossingTypeNo;
	 
	if(start.m_x < pnt.m_x + FreeREP::CONFUSION || end.m_x < pnt.m_x + FreeREP::CONFUSION)
		return CrossingTypeMaybe;
		
	if(start.m_y > pnt.m_y - FreeREP::CONFUSION && start.m_y < pnt.m_y + FreeREP::CONFUSION)
		return CrossingTypeMaybe;
		
	if(end.m_y > pnt.m_y - FreeREP::CONFUSION && end.m_y < pnt.m_y + FreeREP::CONFUSION)
		return CrossingTypeMaybe;

	return CrossingTypeYes;
}

//Determines if pnt is inside of a simple polygon
//polygon must be ordered, but not necessarily cw or ccw
bool Inside(Geom_Vec3 pnt, std::vector<Geom_Vec3> &poly)
{
	int crossings=0;
	bool crossed=false;
	for(size_t i=0; i < poly.size(); i++)
	{
		EnumCrossingType type;
		if(i == poly.size() - 1)
			type = Crosses(pnt,poly[i],poly[0]);
		else
			type = Crosses(pnt,poly[i],poly[i+1]);
		switch(type)
		{
			//TODO: maybe's need to be handled more rubustly
			case CrossingTypeMaybe:
				break;
			case CrossingTypeNo:
				break;
			case CrossingTypeYes:
				crossings++;
				break;
		}
	}
	
	return crossings % 2;
}

double DistanceToLineSegment(Geom_Vec3 pnt, Geom_Vec3 l1, Geom_Vec3 l2)
{
	double norm = (l1 - pnt).Norm();
	double u = ((l2.m_x - pnt.m_x) * (l1.m_x - pnt.m_x) + (l2.m_y - pnt.m_y) * (l1.m_y - pnt.m_y)) / (norm * norm);
	if(u < 0)
		return (l1 - pnt).Norm();
	if(u > 1)
		return (l2 - pnt).Norm();
	return (((l2 - l1) * u + l1) - pnt).Norm(); 
}

double DistanceToPolygon(Geom_Vec3 pnt, std::vector<Geom_Vec3> &poly)
{
	double distance = REALLY_BIG;
	for(size_t i=0; i < poly.size(); i++)
	{
		double d;
		if(i == poly.size() - 1)
			d = DistanceToLineSegment(pnt,poly[i],poly[0]);
		else
			d = DistanceToLineSegment(pnt,poly[i],poly[i+1]);
		if(d < distance)
			distance = d;
	}
	return distance;
}