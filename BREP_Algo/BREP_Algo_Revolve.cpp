//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Revolve.cpp - Author Jon Pry

#include "FreeREP.h"
#include "BREP_Algo_Revolve.h"
#include "Topo_Edge.h"
#include "Topo_Arc.h"
#include "Topo_Face_Planar.h"
#include "BREP_Algo_Make_Cone.h"

#include <vector>

Topo_Face* GeneratePlanarSurface(Topo_Line *directrix, Geom_Vec3 pnt)
{
	Geom_Line dline = directrix->GetLine();
	
	Geom_Vec3 loc = dline.ClosestPoint(pnt);
	Geom_Plane plane(loc,dline.Direction());
	
	return new Topo_Face_Planar(plane);	
}

void GenerateConicSurface(Topo_Line *directrix, Topo_Line *generatrix, std::vector<Topo_Face*> &ret, double start, double end)
{
	Geom_Line dline = directrix->GetLine();
	Geom_Line gline = generatrix->GetLine();
	
//	double l1 = dline.Length();
//	double l2 = gline.Length();
	
	Geom_Vec3 mpt = gline.MidPoint();
	
	Geom_Vec3 loc = dline.ClosestPoint(mpt);
	Geom_Vec3 zdir = dline.Direction().Normalized();
	
	Geom_Vec3 xdir = (mpt-loc).Normalized();
	
	
	double r1 = dline.DistanceToLine(generatrix->GetStart());
	double r2 = dline.DistanceToLine(generatrix->GetEnd());
	
	//R1 has to be for the point closest to the directrix start
	//so we switch if necessary
	
	if(generatrix->GetStart().Distance(directrix->GetStart()) > generatrix->GetEnd().Distance(directrix->GetStart()))
	{
		double t = r1;
		r1 = r2;
		r2 = t;	
	}
	
	double length = dline.ClosestPoint(generatrix->GetStart()).Distance(dline.ClosestPoint(generatrix->GetEnd()));
	
	std::vector<Topo_Face*> faces = MakeConeSectionSkeleton(Geom_Ax2(loc,zdir,xdir),r1,r2,length,start,end);
	
	for(size_t i=0; i < faces.size(); i++)
		ret.push_back(faces[i]);
}

std::vector<Topo_Face*> RevolveWireSkeleton(Topo_Line *directrix, Topo_Wire *wire, double start, double end)
{
	std::vector<Topo_Face*> ret;
	
	if(dynamic_cast<Topo_Line*>(wire))
	{
		Topo_Line* line = (Topo_Line*)wire;
		
		//Check is generatrix line is coplanar to directrix line
		
		//First check if either endpoint of the generatrix lies on the directrix. If it does
		//then we are definately coplanar
		
		Geom_Line dline = directrix->GetLine();
		if(dline.IsOn(line->GetStart()) || dline.IsOn(line->GetEnd()))
		{
			//We are coplanar
			
			//Check to see if this is a flat surface
			if(ISZERO(dline.Direction() * line->GetLine().Direction()))
			{
				ret.push_back(GeneratePlanarSurface(directrix,line->GetStart()));	
			}
			else
			{
				GenerateConicSurface(directrix,line,ret,start,end);
			}
		}
		else
		{
			Geom_Plane plane(directrix->GetStart(),directrix->GetEnd(),line->GetStart());
			Geom_Vec3 projection = plane.MapPoint(line->GetEnd());
			if(ISZERO(projection.m_z))
			{
				//We are coplanar				

				//Check to see if this is a flat surface
				if(ISZERO(dline.Direction() * line->GetLine().Direction()))
				{
					ret.push_back(GeneratePlanarSurface(directrix,line->GetStart()));	
				}
				else
				{
					GenerateConicSurface(directrix,line,ret,start,end);
				}

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

Topo_Face_Compound* RevolveSkeleton(Topo_Line *line, Topo_Shape* g, double start, double end)
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
			std::vector<Topo_Face*> faces = RevolveWireSkeleton(line,wire,start,end);
			
			for(size_t i =0; i < faces.size(); i++)
			{
				mface->Add(faces[i]);			
			}			
			
			edge->GetNextWire(&wire,&order);	
		}
	}
	else
	{
		std::vector<Topo_Face*> faces = RevolveWireSkeleton(line,(Topo_Wire*)g,start,end);
		
		for(size_t i =0; i < faces.size(); i++)
		{
			mface->Add(faces[i]);			
		}
	}
	
	return mface;
} 