//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Compound.cpp - Author Jon Pry

#include "Topo_Face_Compound.h"

Topo_Face_Compound::Topo_Face_Compound(Topo_Line *directrix)
{
	m_directrix = directrix;
}

void Topo_Face_Compound::Add(Topo_Face *face)
{
	m_faces.push_back(face);	
} 

Topo_Face* Topo_Face_Compound::GetFaces()
{
	return m_faces[0];	
}

void *Topo_Face_Compound::MakeTranslatedCopy(Geom_Vec3 dir) const
{
	//TODO: implement this, or throw exception
	return 0;
}

void Topo_Face_Compound::Triangulate(double dDeviation, void (*)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const
{
	//We don't actually support triangulation. But there is no sense in crashing when called	
}

void Topo_Face_Compound::Add(Topo_Edge *edge,bool inside)
{
/*	for(size_t i=0; i < m_faces.size(); i++)
	{
		Topo_Wire *wire;
		EnumWireOrder order;
		edge->GetFirstWire(&wire,&order);
		while(wire)
		{
			if(m_faces[i]->Contains(wire))
			{
				//TODO: do something	
			}
			
			edge->GetNextWire(&wire,&order);	
		}	
	}*/
	//edge->Reverse();
	m_faces[0]->Add(edge,inside);
	
	edge->Print();	
}
