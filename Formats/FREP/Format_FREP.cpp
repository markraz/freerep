//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Format_FREP.cpp - Author Jon Pry

//This file implements a reader for a garbage file format
//The FREP format is designed to be able to read in test
//data. The format follows a 1to1 correspondance to internal
//data structures and is thus subject to change without notice
//however, importers for other file formats can be modelled as
//FREP converters.

#include "Topo_Shape.h"
#include "Topo_Arc.h"
#include "Topo_Line.h"
#include "Topo_Edge.h"
#include "Topo_Face_Planar.h"
#include "Topo_Face_Spheric.h"
#include "Topo_Solid.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

std::vector<Topo_Shape*> wires;
std::vector<Topo_Shape*> edges;
std::vector<Topo_Shape*> faces;
std::vector<Topo_Shape*> solids;

std::vector<Topo_Shape*> ReadFREP(const char* filename)
{
	wires.clear();
	edges.clear();
	faces.clear();
	solids.clear();
	
	std::string line;
	std::ifstream file;
	
	file.open (filename, std::ios::in | std::ios::app);
	if(file.is_open())
	{
		while(!file.eof())
		{
			getline (file,line);
					
			if(line.length() < 2)
				continue;
			if(*line.begin() == '#')
				continue;
			if(*line.begin() == ' ')
				continue;
				
			int typeidx = line.find_first_of('(');
			std::string type = line.substr(0,typeidx);
			std::string restofline = line.substr(typeidx+1);
			const char* cstr = restofline.c_str();
			
			int id=0;
			int cidx = 1;
			double vars[256];
			int ivars[256];
			if(type == "LINE")
			{
				sscanf(cstr,"%d)%lf,%lf,%lf,%lf,%lf,%lf",&id,&vars[0],&vars[1],&vars[2],&vars[3],&vars[4],&vars[5]);
				wires.push_back(new Topo_Line(Geom_Vec3(vars[0],vars[1],vars[2]),Geom_Vec3(vars[3],vars[4],vars[5])));
			}
			else if(type == "ARC")
			{
				sscanf(cstr,"%d)%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",&id,&vars[0],&vars[1],&vars[2],&vars[3],&vars[4],&vars[5],&vars[6],&vars[7],&vars[8],&vars[9],&vars[10],&vars[11]);
				wires.push_back(new Topo_Arc(Geom_Ax2(Geom_Vec3(vars[0],vars[1],vars[2]),Geom_Vec3(vars[3],vars[4],vars[5]),Geom_Vec3(vars[6],vars[7],vars[8])),vars[9],vars[10],vars[11]));
			}
			else if(type == "EDGE" || type == "SOLID" || type == "FACE")
			{
				sscanf(cstr,"%d)%lf",&id,&vars[0]);
				sscanf(cstr,"%d)%d",&id,&ivars[0]);
				//read the CSV data
				int idx = restofline.find(',');
				while(idx > 0)
				{
					restofline = restofline.substr(idx+1);
					cstr = restofline.c_str();
					sscanf(cstr,"%lf",&vars[cidx]);
					sscanf(cstr,"%d",&ivars[cidx++]);
					idx = restofline.find(',');
				}
		
			
				if(type == "EDGE")
				{
					Topo_Edge *edge = new Topo_Edge();
					for(int i=0; i < cidx; i++)
					{
						edge->Add((Topo_Wire*)wires[ivars[i]]);
					}		
					edges.push_back(edge);
				}
			
				if(type == "FACE")
				{
					//TODO: should be able to allocate different types
					Topo_Face *face = 0;
					switch(ivars[0])
					{
						case 0:
							face = new Topo_Face_Planar(Geom_Plane(Geom_Vec3(0,0,0),Geom_Vec3(vars[1],vars[2],vars[3])));
							break;
						case 1:
							face = new Topo_Face_Spheric(Geom_Plane(Geom_Vec3(0,0,0),Geom_Vec3(vars[1],vars[2],vars[3])));
							break;
						default:
							break;
					}
					for(int i=4; i < cidx; i++)
					{
						face->Add((Topo_Edge*)edges[ivars[i]]);	
					}	
					faces.push_back(face);
				}
			
				if(type == "SOLID")
				{
					Topo_Solid *solid = new Topo_Solid();
					for(int i=0; i < cidx; i++)
					{
						solid->Add((Topo_Face*)faces[ivars[i]]);
					}
					solids.push_back(solid);
				}
			}
		}
		file.close();	
	}
	
	//Return an array of the highest level objects the FREP contains
	std::vector<Topo_Shape*> ret;
	if(solids.size())
	{
		ret = std::vector<Topo_Shape*>(solids.begin(),solids.end());
	}
	else if(faces.size())
	{
		ret = std::vector<Topo_Shape*>(faces.begin(),faces.end());
	}
	else if(edges.size())
	{
		ret = std::vector<Topo_Shape*>(edges.begin(),edges.end());
	}
	else if(wires.size())
	{
		ret = std::vector<Topo_Shape*>(wires.begin(),wires.end());
	}
	return ret;
}
