//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Revolve.cpp - Author Jon Pry

#include "FreeREP.h"
#include "BREP_Algo_Revolve.h"
#include "Topo_Edge.h"
#include "Topo_Arc.h"

#include <vector>

std::vector<Topo_Face*> RevolveWireSkeleton(Topo_Line *directrix, Topo_Wire *wire)
{
	std::vector<Topo_Face*> ret;
	
	if(dynamic_cast<Topo_Line*>(wire))
	{
		Topo_Line* line = (Topo_Line*)line;
		
		//Check is generatrix line is coplanar to directrix line
		
		//First check if either endpoint of the generatrix lies on the directrix. If it does
		//then we are definately coplanar
		
		Geom_Line dline = directrix->GetLine();
		if(dline.IsOn(line->GetStart()) || dline.IsOn(line->GetEnd()))
		{
			//We are coplanar
			//TODO: generate cylindrical surfaces
		}
		else
		{
			Geom_Plane plane(directrix->GetStart(),directrix->GetEnd(),line->GetStart());
			Geom_Vec3 projection = plane.MapPoint(line->GetEnd());
			if(ISZERO(projection.m_z))
			{
				//We are coplanar				
				//TODO: generate cylindrical surfaces
			}
			else
			{
				//We are not coplanar
				//TODO: cannot revolve this line because we need hyperboloid surfaces
				//probably should throw an exception for now	
			}
		}
	}
	else if(dynamic_cast<Topo_Arc*>(wire))
	{
		Topo_Arc* arc = (Topo_Arc*)arc;
		Geom_Line dline = directrix->GetLine();
		if(dline.IsOn(arc->GetAxis().Location()) && ISZERO(arc->GetAxis().ZDir() * dline.Direction()))
		{
			//TODO: build spherical surfaces				
		}
		else
		{
			//TODO: surface could ellipsoid, torroidal, or torroidal ellipsoid
			//probably should throw an exception until those are implmented	
		}
	}
	
	return ret;
}

Topo_Face_Compound* RevolveSkeleton(Topo_Line *line, Topo_Shape* g)
{
	Topo_Face_Compound *mface = new Topo_Face_Compound();
		
	if(dynamic_cast<Topo_Edge*>(g))
	{
		Topo_Edge *edge = (Topo_Edge*)g;
		Topo_Wire *wire;
		EnumWireOrder order;
		edge->GetFirstWire(&wire,&order);
		while(wire)
		{
			std::vector<Topo_Face*> faces = RevolveWireSkeleton(line,wire);
			
			for(size_t i =0; i < faces.size(); i++)
			{
				mface->Add(faces[i]);			
			}			
			
			edge->GetNextWire(&wire,&order);	
		}
	}
	else
	{
		std::vector<Topo_Face*> faces = RevolveWireSkeleton(line,(Topo_Wire*)g);
		
		for(size_t i =0; i < faces.size(); i++)
		{
			mface->Add(faces[i]);			
		}
	}
	
	return mface;
} 