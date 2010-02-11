//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Format_IGES.cpp - Author Jon Pry 

#include "Topo_Shape.h"
#include "Topo_Line.h"
#include "Topo_Arc.h"
#include "Topo_Face.h"
#include "Topo_Face_Planar.h";
#include "Geom_Transform.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>

std::string globals;
char parm_delimiter;
char record_delimiter;
std::string product_id_from_sender;
std::string filename;
std::string native_system_id;
std::string preprocessor_version;
int number_of_bits_for_int; 
int single_precision_sig_digs;
int max_power_of_ten_double_precision;
int max_power_of_ten_single_precision;
int double_precision_sig_digs;
	
std::string product_id_reciever;
	
double model_space_scale;

int units_flag;
std::string units_name;
	
int max_linewidth_gradations;
double max_linewidth;
	
std::string file_date;
	
std::string author;
std::string organization;

double max_specified_resolution;
double max_coordinate_value;
	
int version_compliance;
int drafting_standard;
	
std::string model_modified_date;
std::string application_protocol;

int ReadReal(double &destination, std::string &source, int index, char delimiter)
{
	std::string doublestring;
	while(source[index] != delimiter)
		doublestring = doublestring + source[index++];
		
	int idx = doublestring.find_first_of('D', 0);
	if(idx>=0)
		doublestring.replace(idx,1,"E");
		
	destination = strtod(doublestring.c_str(),0);
	return index+1;	
}

int ReadInt(std::string &source, int start, int end)
{
	return atoi(source.substr(start,end-start+1).c_str());	
}

std::string ReadString(std::string &source, int start, int end)
{
	return source.substr(start,end-start+1);	
}

int ReadInt(int &destination, std::string &source, int index, char delimiter)
{
	std::string intstring;
	while(source[index] != delimiter && source[index] != record_delimiter)
		intstring = intstring + source[index++];
		
	destination = atoi(intstring.c_str());
	return index+1;	
}

int ReadString(std::string &destination, std::string &source, int index)
{
	int nchars;
	
	if(source[index] == parm_delimiter || source[index] == record_delimiter)
		return index+1;
	
	index = ReadInt(nchars,source,index,'H');
	
	destination = source.substr(index,nchars);
	
	return index + nchars + 1;
}

void PushGlobals(std::string string)
{
	globals = globals + string;	
}

void ParseGlobals()
{
	parm_delimiter = globals[2];
	record_delimiter = globals[6];	
	
	int index = 8;
	index = ReadString(product_id_from_sender, globals, index);
	index = ReadString(filename, globals, index);
	index = ReadString(native_system_id, globals, index);
	index = ReadString(preprocessor_version,globals,index);
	
	index = ReadInt(number_of_bits_for_int,globals,index,parm_delimiter); 
	index = ReadInt(max_power_of_ten_single_precision,globals,index,parm_delimiter);
	index = ReadInt(single_precision_sig_digs,globals,index,parm_delimiter);
	index = ReadInt(max_power_of_ten_double_precision,globals,index,parm_delimiter);
	index = ReadInt(double_precision_sig_digs,globals,index,parm_delimiter);
	
	index = ReadString(product_id_reciever, globals, index);
	
	index = ReadReal(model_space_scale,globals,index,parm_delimiter);
	
	index = ReadInt(units_flag,globals,index,parm_delimiter);
	index = ReadString(units_name, globals, index);
	
	index = ReadInt(max_linewidth_gradations,globals,index,parm_delimiter);
	
	index = ReadReal(max_linewidth,globals,index,parm_delimiter);
	
	index = ReadString(file_date, globals, index);
	
	index = ReadReal(max_specified_resolution,globals,index,parm_delimiter);
	index = ReadReal(max_coordinate_value,globals,index,parm_delimiter);
	
	index = ReadString(author, globals, index);
	index = ReadString(organization, globals, index);
	
	index = ReadInt(version_compliance,globals,index,parm_delimiter);
	index = ReadInt(drafting_standard,globals,index,parm_delimiter);
	
	index = ReadString(model_modified_date, globals, index);
	index = ReadString(application_protocol, globals, index);
}

class DirectoryEntry
{
public:
	DirectoryEntry() { m_shape = NULL; }
	int m_typenumber;
	int m_parm_data;
	int m_structure;
	int m_line_font;
	int m_level;
	int m_view;
	int m_transform;
	int m_label_display;
	int m_status;
	int m_sequence;
	int m_line_weight;
	int m_color;
	int m_parm_lines;
	int m_form;
	std::string m_label;
	int m_subscript;
	
	Topo_Shape *m_shape;
	Geom_Transform *m_trsf;
};

std::vector<DirectoryEntry> directory_entries;

void ParseDE(std::string line1, std::string line2)
{
	DirectoryEntry de;
	de.m_typenumber = ReadInt(line1,0,7);
	de.m_parm_data = ReadInt(line1,8,15);
	de.m_structure = ReadInt(line1,16,23);
	de.m_line_font = ReadInt(line1,24,31);
	de.m_level = ReadInt(line1,32,39);
	de.m_view = ReadInt(line1,40,47);
	de.m_transform = ReadInt(line1,48,55);
	de.m_label_display = ReadInt(line1,56,63);
	de.m_status = ReadInt(line1,64,71);
	de.m_sequence = ReadInt(line1,73,79);
	
	de.m_line_weight = ReadInt(line2,8,15);
	de.m_color = ReadInt(line2,16,23);
	de.m_parm_lines = ReadInt(line2,24,31);
	de.m_form = ReadInt(line2,32,39);
	de.m_label = ReadString(line2,56,63);
	de.m_subscript = ReadInt(line2,64,71);
	
	directory_entries.push_back(de);
}

Geom_Vec3 TransformPoint(Geom_Vec3 pnt, DirectoryEntry* de)
{
	if(de->m_transform == 0)
		return pnt;
	
	DirectoryEntry *trsf = &directory_entries[(de->m_transform-1)/2];
	return trsf->m_trsf->Transform(pnt);	
}

Geom_Vec3 RotatePoint(Geom_Vec3 pnt, DirectoryEntry* de)
{
	if(de->m_transform == 0)
		return pnt;
	
	DirectoryEntry *trsf = &directory_entries[(de->m_transform-1)/2];
	return trsf->m_trsf->Rotate(pnt);	
}

void ParseMatrix(std::string &line, int index, DirectoryEntry* de)
{ 
	double R11,R12,R13,T1,R21,R22,R23,T2,R31,R32,R33,T3;
	
	index = ReadReal(R11,line,index,parm_delimiter);
	index = ReadReal(R12,line,index,parm_delimiter);
	index = ReadReal(R13,line,index,parm_delimiter);
	
	index = ReadReal(T1,line,index,parm_delimiter);
	
	index = ReadReal(R21,line,index,parm_delimiter);
	index = ReadReal(R22,line,index,parm_delimiter);
	index = ReadReal(R23,line,index,parm_delimiter);
	
	index = ReadReal(T2,line,index,parm_delimiter);
	
	index = ReadReal(R31,line,index,parm_delimiter);
	index = ReadReal(R32,line,index,parm_delimiter);
	index = ReadReal(R33,line,index,parm_delimiter);
	
	index = ReadReal(T3,line,index,parm_delimiter);
	
	Geom_Matrix mat;
	mat(0,0) = R11;
	mat(0,1) = R12;
	mat(0,2) = R13;
	mat(1,0) = R21;
	mat(1,1) = R22;
	mat(1,2) = R23;
	mat(2,0) = R31;
	mat(2,1) = R32;
	mat(2,2) = R33;
	de->m_trsf = new Geom_Transform(mat,Geom_Vec3(T1,T2,T3));
}

void ParseArc(std::string &line, int index, DirectoryEntry* de)
{
	double z,cx,cy,sx,sy,ex,ey;
	index = ReadReal(z,line,index,parm_delimiter);
		
	index = ReadReal(cx,line,index,parm_delimiter);
	index = ReadReal(cy,line,index,parm_delimiter);
	
	index = ReadReal(sx,line,index,parm_delimiter);
	index = ReadReal(sy,line,index,parm_delimiter);
	
	index = ReadReal(ex,line,index,parm_delimiter);
	index = ReadReal(ey,line,index,parm_delimiter);
	
	double dx = sx - cx;
	double dy = sy - cy;
	double r = sqrt(dx*dx+dy*dy);
	
	double sa = atan2(sy-cy,sx-cx);
	double ea = atan2(ey-cy,ex-cx);
	
	Geom_Vec3 loc = TransformPoint(Geom_Vec3(0,0,z),de);
	Geom_Vec3 zvec = RotatePoint(Geom_Vec3(0,0,1),de);
	Geom_Vec3 xvec = RotatePoint(Geom_Vec3(1,0,0),de);
	
	de->m_shape = new Topo_Arc(Geom_Ax2(loc,zvec,xvec),r,sa,ea);
}

void ParseLine(std::string &line, int index, DirectoryEntry* de)
{
//	printf("%s\n",line.c_str());
	
	double x1,y1,z1,x2,y2,z2;
	index = ReadReal(x1,line,index,parm_delimiter);	
	index = ReadReal(y1,line,index,parm_delimiter);
	index = ReadReal(z1,line,index,parm_delimiter);
	
	index = ReadReal(x2,line,index,parm_delimiter);
	index = ReadReal(y2,line,index,parm_delimiter);
	index = ReadReal(z2,line,index,parm_delimiter);
	
	Geom_Vec3 p1 = TransformPoint(Geom_Vec3(x1,y1,z1),de);
	Geom_Vec3 p2 = TransformPoint(Geom_Vec3(x2,y2,z2),de);
	de->m_shape = new Topo_Line(p1,p2);
}

void ParsePlane(std::string &line, int index, DirectoryEntry *de)
{
	bool m_inverted = de->m_form == -1;
	double a,b,c,d,x,y,z,size;
	int ptr;
	index = ReadReal(a,line,index,parm_delimiter);	
	index = ReadReal(b,line,index,parm_delimiter);
	index = ReadReal(c,line,index,parm_delimiter);
	index = ReadReal(d,line,index,parm_delimiter);
	
	index = ReadInt(ptr,line,index,parm_delimiter);
	
	index = ReadReal(x,line,index,parm_delimiter);
	index = ReadReal(y,line,index,parm_delimiter);
	index = ReadReal(z,line,index,parm_delimiter);
	
	index = ReadReal(size,line,index,parm_delimiter);
	
	de->m_shape = new Topo_Face_Planar(Geom_Plane(a,b,c,d));
}

void ParseCurveOnParametricSurface(std::string &line, int index, DirectoryEntry *de)
{
	int creation;
	int surface;
	int parametriccurve;
	int worldcurve;
	int preferredrep;
	
	index = ReadInt(creation,line,index,parm_delimiter);	
	index = ReadInt(surface,line,index,parm_delimiter);
	index = ReadInt(parametriccurve,line,index,parm_delimiter);
	index = ReadInt(worldcurve,line,index,parm_delimiter);
	index = ReadInt(preferredrep,line,index,parm_delimiter);

	//TODO: if we are given a parametric curve instead of a world curve
	//it must be converted into world space. Unfortunately at this
	//point we don't even know what parametric surface the curve belongs to.
	de->m_shape = directory_entries[(worldcurve-1)/2].m_shape;
}

void ParseTrimmedSurface(std::string &line, int index, DirectoryEntry* de)
{
	int surface;
	int is_outer;
	int n_inner_curves;
	int p_outer;

	index = ReadInt(surface,line,index,parm_delimiter);
	index = ReadInt(is_outer,line,index,parm_delimiter);
	index = ReadInt(n_inner_curves,line,index,parm_delimiter);
	index = ReadInt(p_outer,line,index,parm_delimiter);
		
	std::vector<int> inner_curves;
	int inner_curve=-1;
	for(int i=0; i < n_inner_curves; i++)
	{
		index = ReadInt(inner_curve,line,index,parm_delimiter);			
		inner_curves.push_back(inner_curve);
	}

	Topo_Face *psurface = (Topo_Face*)directory_entries[(surface-1)/2].m_shape;
	if(!psurface)
	{
		//TODO: throw some kind of error, we didn't build a surface for this curve
		return;	
	}
	
	if(p_outer)
	{
		psurface->Add((Topo_Edge*)directory_entries[(p_outer-1)/2].m_shape,false);	
	}
	
	for(size_t i=0; i < inner_curves.size(); i++)
	{
		int entry = (inner_curves[i]-1)/2;
		psurface->Add((Topo_Edge*)directory_entries[entry].m_shape,true);	
	}	
}

void ParseComposite(std::string &line, int index, DirectoryEntry* de)
{
//	printf("%s\n",line.c_str());
	
	std::vector<int> elements;
	int element=-1;
	int nelements;	
	index = ReadInt(nelements,line,index,parm_delimiter);
	index = ReadInt(element,line,index,parm_delimiter);
	while(element > 0)
	{
		elements.push_back(element);
		index = ReadInt(element,line,index,parm_delimiter);			
	}
	
	//return; 
		
	de->m_shape = new Topo_Edge();
	
	for(size_t i=0; i < elements.size(); i++)
	{
		int entry = (elements[i]-1)/2;
		((Topo_Edge*)de->m_shape)->Add((Topo_Wire*)directory_entries[entry].m_shape);	
	}
}

void ParseParm(std::string &line, int de_pointer)
{
	//printf("%s\n",line.c_str());
	
	int index = 0;
	int entity_type=0;
	
	index = ReadInt(entity_type,line,index,parm_delimiter);
	
	DirectoryEntry *de = &directory_entries[(de_pointer-1)/2];
	
	switch(entity_type)
	{
		case 100:
			ParseArc(line,index,de);
			break;
		case 102:
			ParseComposite(line,index,de);
			break;
		case 108:
			ParsePlane(line,index,de);
			break;
		case 110:
			ParseLine(line,index,de);
			break;
		case 124:
			ParseMatrix(line,index,de);
			break;
		case 142:
			ParseCurveOnParametricSurface(line,index,de);
			break;
		case 144:
			ParseTrimmedSurface(line,index,de);
			break;
		default:
			break;	
	}
}

std::vector<Topo_Shape*> ReadIGES(const char* filename)
{
	std::string line;
	std::ifstream file;
	
	bool gotstart = false;
	bool gotend = false;
	bool parsedglobals = false;
	
	int de_count=0;
	std::string lastline;
	
	std::string parmline;
	
	file.open (filename, std::ios::in | std::ios::app);
	if(file.is_open())
	{
		while(!file.eof())
		{
			char cline[128];
			file.getline (cline,128);
			line = std::string(cline);
			int length = line.length();
			if(length != 80)
				continue;
				
			switch(line[72])
			{
				case 'S':
					if(!gotstart)
						gotstart = true;
					if(gotend)
						throw 21;
					break;
				case 'G':
					if(!gotstart)
						throw 20;
					if(gotend)
						throw 21;
					PushGlobals(line.substr(0,72));
					break;
				case 'D':
					if(!gotstart)
						throw 20;
					if(gotend)
						throw 21;
					if(!parsedglobals)
					{
						parsedglobals = true;
						ParseGlobals();	
					}
					if(de_count++%2)
						ParseDE(lastline,line);
					break;
				case 'P':
					if(!gotstart)
						throw 20;
					if(gotend)
						throw 21;
					if(!parsedglobals)
					{
						parsedglobals = true;
						ParseGlobals();	
					}
					
					parmline = parmline + line.substr(0,64);
					if(line.find(';') != std::string::npos)
					{
						ParseParm(parmline,ReadInt(line,65,71));
						parmline = "";	
					}
					
					
					break;
				case 'T':
					if(!gotstart)
						throw 20;
					if(!gotend)
						gotend = true;
					else
						throw 21;
				default:
					break;
			}
			lastline = line;
		}
		file.close();
	}	
	
	std::vector<Topo_Shape*> wires;
	std::vector<Topo_Shape*> edges;
	std::vector<Topo_Shape*> faces;
	
	for(int i=0; i < directory_entries.size(); i++)
	{
		DirectoryEntry *de = &directory_entries[i];
		if(!de->m_shape)
			continue;
		if(dynamic_cast<Topo_Wire*>(de->m_shape))
			wires.push_back(de->m_shape);
		
		if(dynamic_cast<Topo_Edge*>(de->m_shape))
			edges.push_back(de->m_shape);	
		
		if(dynamic_cast<Topo_Face*>(de->m_shape))
			faces.push_back(de->m_shape);	
	}
	
	if(faces.size())
		return faces;
	if(edges.size())
		return edges;
	if(wires.size())
		return wires;
	return std::vector<Topo_Shape*>();
}