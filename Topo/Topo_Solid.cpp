//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Solid.cpp - Author Jon Pry 

#include "Topo_Solid.h"

void Topo_Solid::Add(Topo_Face *face)
{
    m_faces.push_back(face);
}

void Topo_Solid::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3 &pnt, const Geom_Vec3 &norm))
{
    std::list<Topo_Face*>::iterator it;
    for(it = m_faces.begin(); it != m_faces.end(); it++)
    {
        (*it)->Triangulate(dDeviation,pRet);
    }
}

Topo_Face* Topo_Solid::GetFirstFace()
{
    m_faces_it = m_faces.begin();
    return GetNextFace();
}

Topo_Face* Topo_Solid::GetNextFace()
{
    if(m_faces_it != m_faces.end())
        return *m_faces_it++;
    return 0;
}