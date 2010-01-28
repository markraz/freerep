//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Format_IGES.cpp - Author Jon Pry 

#include "Topo_Shape.h"

#include <stdio.h>
#include <stdlib.h>

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
int double_precision_sig_digs;
	
std::string product_id_reciever;
	
int units_flag;
std::string units_name;
	
int max_linewidth_gradations;
	
std::string file_date;
	
std::string author;
std::string organization;
	
int version_compliance;
	
std::string model_modified_date;
std::string application_protocol;


int ReadInt(int &destination, std::string &source, int index, char delimiter)
{
	std::string intstring;
	while(source[index] != delimiter)
		intstring = intstring + source[index++];
		
	destination = atoi(intstring.c_str());
	return index+1;	
}

int ReadString(std::string &destination, std::string &source, int index)
{
	int nchars;
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
	index = ReadInt(single_precision_sig_digs,globals,index,parm_delimiter);
	index = ReadInt(max_power_of_ten_double_precision,globals,index,parm_delimiter);
	index = ReadInt(double_precision_sig_digs,globals,index,parm_delimiter);
	
	index = ReadString(product_id_reciever, globals, index);
	
	//TODO: read real for model space scale
	
	index = ReadInt(units_flag,globals,index,parm_delimiter);
	index = ReadString(units_name, globals, index);
	
	index = ReadInt(max_linewidth_gradations,globals,index,parm_delimiter);
	
	//TODO: read real for max line width
	
	index = ReadString(file_date, globals, index);
	
	//TODO: read real for minimum specified resolution
	//TODO: read real for max coordinate value in file
	
	index = ReadString(author, globals, index);
	index = ReadString(organization, globals, index);
	
	index = ReadInt(version_compliance,globals,index,parm_delimiter);
	
	index = ReadString(model_modified_date, globals, index);
	index = ReadString(application_protocol, globals, index);
}

std::vector<Topo_Shape*> ReadIGES(const char* filename)
{
	std::string line;
	std::ifstream file;
	
	bool gotstart = false;
	bool gotend = false;
	bool parsedglobals = false;
	
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
		}
		file.close();
	}	
}