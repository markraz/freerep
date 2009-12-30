//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//FREP.cpp - Author Jon Pry

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
#include "Topo_Face.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

std::vector<Topo_Wire*> wires;
std::vector<Topo_Edge*> edges;
std::vector<Topo_Face*> faces;

std::vector<Topo_Shape*> ReadFREP(const char* filename)
{
	wires.clear();
	edges.clear();
	faces.clear();
	
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
			const char* cstr = type.c_str();
			
			int id=0;
			double vars[12];
			if(type == "LINE")
			{
				sscanf(cstr,"%d)%lf,%lf,%lf,%lf,%lf,%lf",&id,&vars[0],&vars[1],&vars[2],&vars[3],&vars[4],&vars[5]);
				wires.push_back(new Topo_Line(Geom_Vec3(vars[0],vars[1],vars[2]),Geom_Vec3(vars[3],vars[4],vars[5])));
			}
			if(type == "ARC")
			{
				sscanf(cstr,"%d)%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",&id,&vars[0],&vars[1],&vars[2],&vars[3],&vars[4],&vars[5],&vars[6],&vars[7],&vars[8],&vars[9],&vars[10],&vars[11]);
				wires.push_back(new Topo_Arc(Geom_Ax2(Geom_Vec3(vars[0],vars[1],vars[2]),Geom_Vec3(vars[3],vars[4],vars[5]),Geom_Vec3(vars[6],vars[7],vars[8])),vars[9],vars[10],vars[11]));
			}
			
			if(type == "EDGE" || type == "SOLID" || type == "FACE")
			{
				sscanf(cstr,"%d",&id);
				//TODO: read the CSV data	
			}
			
			if(type == "EDGE")
			{
				
			}
			if(type == "SOLID")
			{
				
			}
			if(type == "FACE")
			{
				
			}
		}
		file.close();	
	}
}
