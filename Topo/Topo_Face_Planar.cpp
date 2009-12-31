//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face_Planar.cpp - Author Jon Pry 

#include "Topo_Face_Planar.h"
#include "Geom_Plane.h"
#include "Topo_Edge.h"
#include "Tess_Planar.h"

#include <vector>

Topo_Face_Planar::Topo_Face_Planar(Geom_Plane plane)
{
	m_plane = plane;	
}

Topo_Face_Planar::Topo_Face_Planar(const ICanAssociate *associate):Topo_Face(associate)
{
	m_plane = ((Topo_Face_Planar*)associate)->m_plane;
}

std::vector<std::vector<Geom_Vec3> > topo_face_vertices;
int topo_face_current_edge;

void topo_face_vertex_absorber(const Geom_Vec3 &pnt)
{
    topo_face_vertices[topo_face_current_edge].push_back(pnt);
}

void Topo_Face_Planar::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const
{
    Geom_Plane plane = GetPlane();

    topo_face_vertices.clear();
    topo_face_vertices.resize(m_edges.size());

    topo_face_current_edge = 0;
    std::list<Topo_Edge*>::const_iterator it;
    for(it = m_edges.begin(); it != m_edges.end(); it++)
    {
        Topo_Edge *edge = *it;
        edge->GetVertices(dDeviation, topo_face_vertex_absorber);

        topo_face_current_edge++;
    }

    size_t total_size=0;
    int *sizes = new int[topo_face_vertices.size()];
    for(size_t i=0; i < topo_face_vertices.size(); i++)
    {
        sizes[i] = topo_face_vertices[i].size()-2;
        total_size += sizes[i];
    }

    double (*vertices)[2] = new double[total_size + 1][2];
    double (*uvertices)[3] = new double[total_size + 1][3];
    int (*triangles)[3] = new int[total_size * 8][3];
    size_t cvertex = 1;
    for(size_t i=0; i < topo_face_vertices.size(); i++)
    {
        for(size_t j = 0; j < (unsigned int)sizes[i]; j++)
        {
            Geom_Vec3 p = topo_face_vertices[i][j];
            //TODO: project onto the faces plane

            uvertices[cvertex][0] = p.m_x;
            uvertices[cvertex][1] = p.m_y;
            uvertices[cvertex][2] = p.m_z;

            Geom_Vec3 mp = plane.MapPoint(p);
            vertices[cvertex][0] = mp.m_x;
            vertices[cvertex][1] = mp.m_y;

            cvertex++;
        }
    }

#ifdef TRI_DEBUG
    printf("%d\n", (int)topo_face_vertices.size());

    for(size_t i=0; i < topo_face_vertices.size();i++)
    {
        printf("%d\n",sizes[i]);
    }

    for(size_t i=1; i < cvertex; i++)
    {
        printf("%lf,%lf  %lf,%lf,%lf \n",vertices[i][0],vertices[i][1],uvertices[i][0],uvertices[i][1],uvertices[i][2]);
    }
#endif

    int ntris = triangulate_polygon(topo_face_vertices.size(),sizes, vertices,triangles);

    for(int i=0; i < ntris; i++)
    {
        pRet(Geom_Vec3(uvertices[triangles[i][0]][0],uvertices[triangles[i][0]][1],uvertices[triangles[i][0]][2]),plane.GetNorm());
        pRet(Geom_Vec3(uvertices[triangles[i][1]][0],uvertices[triangles[i][1]][1],uvertices[triangles[i][1]][2]),plane.GetNorm());
        pRet(Geom_Vec3(uvertices[triangles[i][2]][0],uvertices[triangles[i][2]][1],uvertices[triangles[i][2]][2]),plane.GetNorm());
    }

    delete sizes;
    delete vertices;
    delete triangles;
    delete uvertices;
}

void *Topo_Face_Planar::MakeTranslatedCopy(Geom_Vec3 dir) const
{
    Topo_Face *nface = new Topo_Face_Planar(this);
    std::list<Topo_Edge*>::const_iterator it;
    for(it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        nface->Add((Topo_Edge*)(*it)->MakeTranslatedCopy(dir));
    }

    return nface;
}

double Topo_Face_Planar::Area()
{
	//TODO: implement me	
}