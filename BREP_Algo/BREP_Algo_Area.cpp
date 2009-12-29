//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Area.cpp - Author Jon Pry

//This is the outer loop for our implementation of Green's theorum

#include "BREP_Algo_Area.h"
#include "Topo_Edge.h"
#include "Topo_Wire.h"
#include "Geom_Plane.h"

double BrepAlgoArea(Topo_Edge *edge)
{
	double area=0;
	
	Topo_Wire *wire;
	EnumWireOrder order;
	EnumWireOrder firstorder;
	Topo_Wire *first=0;
	
	Geom_Plane plane = edge->GetPlane();
	edge->GetFirstWire(&wire,&order);
	while(edge)
	{
		if(!first)
		{
			first = wire;
			firstorder = order;
		}
			
		area += wire->GetArea(order,plane);	
		
     	edge->GetNextWire(&wire,&order);
	}
 
	return area;	
}

double BrepAlgoArea(Topo_Face *face)
{
	double area = 0;
	
	Topo_Edge *edge = face->GetFirstEdge();
	while(edge)
	{
		area += BrepAlgoArea(edge);
	}
	return area;	
}