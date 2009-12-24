//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//BREP_Algo_Extrude.cpp - Author Jon Pry 

#include "BREP_Algo_Extrude.h"
#include "Topo_Line.h"
#include "Topo_Arc.h"
#include "Topo_Face_Conic.h"
#include "Topo_Face_Planar.h"

Topo_Face *CreateExtrudedFace(Topo_Wire* wire, EnumWireOrder direction, Topo_Wire* wire_2,Geom_Vec3 &avertex,Geom_Vec3 &a_2vertex,Geom_Vec3 &bvertex,Geom_Vec3 &b_2vertex);

Topo_Solid* BrepAlgoExtrude(Topo_Face *face, Geom_Vec3 dir)
{
    //TODO: Check that face is coplanar and dir does not lie on the plane. Different algorithms required when those are not true

    //Create a new face which is a translated copy of the old
    Topo_Face *nface = (Topo_Face*)face->MakeTranslatedCopy(dir);

    Topo_Solid *solid = new Topo_Solid();
    solid->Add(face);
    solid->Add(nface);

    Topo_Wire *wire;
    EnumWireOrder direction;
    nface->GetFirstWire(&wire,&direction);

    Geom_Vec3 startvertex;
    Geom_Vec3 avertex;
    Geom_Vec3 bvertex;
    Geom_Vec3 a_2vertex;
    Geom_Vec3 b_2vertex;

    while(wire)
    {
        //TODO: this is only correct if wire is a line

        Topo_Wire *wire_2 = (Topo_Wire*)wire->GetParent();

        if(direction == AFirst)
        {
            avertex = wire->GetStart();
            bvertex = wire->GetEnd();
            a_2vertex = wire_2->GetStart();
            b_2vertex = wire_2->GetEnd();
        }
        else
        {
            avertex = wire->GetEnd();
            bvertex = wire->GetStart();
            a_2vertex = wire_2->GetEnd();
            b_2vertex = wire_2->GetStart();
        }

        Topo_Face *aface = CreateExtrudedFace(wire,direction,wire_2,avertex,a_2vertex,bvertex,b_2vertex);

        solid->Add(aface);

        nface->GetNextWire(&wire,&direction);
    }


    return solid;
}

Topo_Face *CreateExtrudedFace(Topo_Wire* wire, EnumWireOrder direction, Topo_Wire* wire_2,Geom_Vec3 &avertex,Geom_Vec3 &a_2vertex,Geom_Vec3 &bvertex,Geom_Vec3 &b_2vertex)
{
    if(dynamic_cast<Topo_Line*>(wire))
    {

        Topo_Edge *nedge = new Topo_Edge();

        nedge->Add(wire,direction);
        nedge->Add(new Topo_Line(bvertex,b_2vertex));
        nedge->Add(wire_2);
        nedge->Add(new Topo_Line(avertex,a_2vertex));
        
        Topo_Face* aface = new Topo_Face_Planar();
        aface->Add(nedge);

        return aface;
    }
    else
    {
        return new Topo_Face_Conic((Topo_Arc*)wire,(Topo_Arc*)wire_2);
    }
}