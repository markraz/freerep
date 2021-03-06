//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//Topo_Face.cpp - Author Jon Pry 

#include "Topo_Face.h"
#include "Tess_Planar.h"

#include <stdio.h>
#include <vector>

#define TRI_DEBUG

Topo_Face::Topo_Face()
{
	
}

Topo_Face::Topo_Face(const ICanAssociate* associate):ICanAssociate(associate)
{
	
}

Geom_Plane Topo_Face::GetPlane() const
{
	return m_plane;
}

double Topo_Face::Area()
{
	//TODO: throw not implemented
	return 0;	
}

void Topo_Face::Add(Topo_Edge *edge, bool inside)
{
    m_edges.push_back(OrientedEdge(edge,inside?EdgeInside:EdgeOutside));
    edge->AddParent(this);
}


std::vector<std::vector<std::pair<Geom_Vec3,Geom_Vec3> > > topo_face_vertices;
int topo_face_current_edge;

void topo_face_vertex_absorber(const Geom_Vec3 &pnt, const Geom_Vec3 &derivitive)
{
    topo_face_vertices[topo_face_current_edge].push_back(std::pair<Geom_Vec3,Geom_Vec3> (pnt,derivitive));
}

Geom_Vec3 Topo_Face::ParameterizePoint(Geom_Vec3 p, Geom_Vec3 prev) const
{
	return m_plane.MapPoint(p);	
}

void Topo_Face::Triangulate(double dDeviation, void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm)) const
{
	if(!m_edges.size())
		return;
	
    Geom_Plane plane = GetPlane();

    topo_face_vertices.clear();
    topo_face_vertices.resize(m_edges.size());

    topo_face_current_edge = 0;
    std::list<OrientedEdge>::const_iterator it;
    for(it = m_edges.begin(); it != m_edges.end(); it++)
    {
        Topo_Edge *edge = (*it).m_edge;
        edge->GetVertices(dDeviation, topo_face_vertex_absorber);

        topo_face_current_edge++;
    }
    
    //We need to concatenate points for which the parameterizations are coincident
    std::vector<std::vector<std::pair<Geom_Vec3,Geom_Vec3> > > new_topo_face_vertices;
    for(size_t i=0; i < topo_face_vertices.size(); i++)
    {
    	bool gotfirst=false;
    	Geom_Vec3 lastparm;
    	Geom_Vec3 start;
    	new_topo_face_vertices.resize(i+1);
    	for(size_t j=0; j < topo_face_vertices[i].size(); j++)
    	{
    		std::pair<Geom_Vec3,Geom_Vec3> pair = topo_face_vertices[i][j];
    		Geom_Vec3 parm = ParameterizePoint(pair.first, pair.second);
     		if(gotfirst && (!(lastparm == parm)) && !(start == parm))
    			new_topo_face_vertices[i].push_back(pair);
    			
    		if(!gotfirst)
    		{
    			new_topo_face_vertices[i].push_back(pair);
    			start = parm;
    			gotfirst=true;	
    		}
    		
    		lastparm = parm;    		
    	}
    }
    
    //We need to enfore the orientation order of the vertices now
    topo_face_current_edge = 0;
    for(it = m_edges.begin(); it != m_edges.end(); it++)
    {
        EdgeOrientation orientation = (*it).m_orientation;
        double area = 0;
        
        for(size_t i=0; i < new_topo_face_vertices[topo_face_current_edge].size(); i++)
        {
        	std::pair<Geom_Vec3,Geom_Vec3> Bp = new_topo_face_vertices[topo_face_current_edge][i];
        	std::pair<Geom_Vec3,Geom_Vec3> Ap = new_topo_face_vertices[topo_face_current_edge][i==0?(new_topo_face_vertices[topo_face_current_edge].size()-1):(i-1)];
        	
        	Geom_Vec3 B = ParameterizePoint(Bp.first,Bp.second);
        	Geom_Vec3 A = ParameterizePoint(Ap.first,Ap.second);
        	area += A.m_x * B.m_y - A.m_y * B.m_x;	
        }

		if((area < 0 && (orientation == EdgeOutside)) || (area > 0 && (orientation == EdgeInside)))
		{
			//Reverse the points
			for(size_t j=0; j < new_topo_face_vertices[topo_face_current_edge].size()/2; j++)
			{
				std::pair<Geom_Vec3,Geom_Vec3> temp = new_topo_face_vertices[topo_face_current_edge][j];
				new_topo_face_vertices[topo_face_current_edge][j] = new_topo_face_vertices[topo_face_current_edge][new_topo_face_vertices[topo_face_current_edge].size()-1-j];
				new_topo_face_vertices[topo_face_current_edge][new_topo_face_vertices[topo_face_current_edge].size()-1-j] = temp;
			}
		}
			
        topo_face_current_edge++;
    }
    


    size_t total_size=0;
    int *sizes = new int[new_topo_face_vertices.size()];
    for(size_t i=0; i < new_topo_face_vertices.size(); i++)
    {
        sizes[i] = new_topo_face_vertices[i].size();
        total_size += sizes[i];
    }

    double (*vertices)[2] = new double[total_size + 1][2];
    double (*uvertices)[3] = new double[total_size + 1][3];
    int (*triangles)[3] = new int[total_size * 8][3];
    size_t cvertex = 1;
    for(size_t i=0; i < new_topo_face_vertices.size(); i++)
    {
        for(size_t j = 0; j < (unsigned int)sizes[i]; j++)
        {
            std::pair<Geom_Vec3,Geom_Vec3> pair = new_topo_face_vertices[i][j];
            Geom_Vec3 p = pair.first;

            uvertices[cvertex][0] = p.m_x;
            uvertices[cvertex][1] = p.m_y;
            uvertices[cvertex][2] = p.m_z;

			Geom_Vec3 mp = ParameterizePoint(p, pair.second);
            vertices[cvertex][0] = mp.m_x;
            vertices[cvertex][1] = mp.m_y;

            cvertex++;
        }
    }

#ifdef TRI_DEBUG
    printf("%d\n", (int)new_topo_face_vertices.size());

    for(size_t i=0; i < new_topo_face_vertices.size();i++)
    {
        printf("%d\n",sizes[i]);
    }

    for(size_t i=1; i < cvertex; i++)
    {
        printf("%lf,%lf  %lf,%lf,%lf \n",vertices[i][0],vertices[i][1],uvertices[i][0],uvertices[i][1],uvertices[i][2]);
    }
#endif

    int ntris = triangulate_polygon(new_topo_face_vertices.size(),sizes, vertices,triangles);

    for(int i=0; i < ntris; i++)
    {
    	std::vector<Geom_Vec3> uvecs;
    	uvecs.push_back(Geom_Vec3(uvertices[triangles[i][0]][0],uvertices[triangles[i][0]][1],uvertices[triangles[i][0]][2]));
    	uvecs.push_back(Geom_Vec3(uvertices[triangles[i][1]][0],uvertices[triangles[i][1]][1],uvertices[triangles[i][1]][2]));
    	uvecs.push_back(Geom_Vec3(uvertices[triangles[i][2]][0],uvertices[triangles[i][2]][1],uvertices[triangles[i][2]][2]));
    	
    	std::vector<Geom_Vec3> nvecs;
    	nvecs.push_back(Geom_Vec3(vertices[triangles[i][0]][0],vertices[triangles[i][0]][1],0));
    	nvecs.push_back(Geom_Vec3(vertices[triangles[i][1]][0],vertices[triangles[i][1]][1],0));
    	nvecs.push_back(Geom_Vec3(vertices[triangles[i][2]][0],vertices[triangles[i][2]][1],0));
    	
        TriangulateI(pRet, uvecs, nvecs, m_plane.GetNorm());
    }

    delete sizes;
    delete vertices;
    delete triangles;
    delete uvertices;
}

std::pair<Geom_Vec3,Geom_Vec3> Topo_Face::ProjectPoint(Geom_Vec3 pnt,Geom_Vec3 norm) const
{
	return std::pair<Geom_Vec3,Geom_Vec3>(m_plane.UnmapPoint(pnt),m_plane.GetNorm());
}

void Topo_Face::TriangulateI(void (*pRet)(const Geom_Vec3&pnt, const Geom_Vec3&norm), std::vector<Geom_Vec3> uvecs, std::vector<Geom_Vec3> nvec, Geom_Vec3 norm) const
{
		pRet(uvecs[0],norm);
        pRet(uvecs[1],norm);
        pRet(uvecs[2],norm);
}

void Topo_Face::GetFirstWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    m_edges_it = m_edges.begin();

    Topo_Wire *wire=0;
    EnumWireOrder order;
    while(!wire)
    {
        if(m_edges_it != m_edges.end())
        {
            Topo_Edge* edge = (*m_edges_it).m_edge;
            edge->GetFirstWire(&wire,&order);
            if(wire)
                break;
        }
        else
            break;
        m_edges_it++;
    }
    *ppwire = wire;
    *porder = order;
}

void Topo_Face::GetNextWire(Topo_Wire **ppwire, EnumWireOrder *porder)
{
    Topo_Wire *wire=0;
    EnumWireOrder order;

    if(m_edges_it != m_edges.end())
    {
       Topo_Edge* edge = (*m_edges_it).m_edge;
       edge->GetNextWire(&wire,&order);
    }

    while(!wire)
    {
        m_edges_it++;

        if(m_edges_it != m_edges.end())
        {
            Topo_Edge* edge = (*m_edges_it).m_edge;
            edge->GetFirstWire(&wire,&order);
            if(wire)
                break;
        }
        else
            break;
    }
    *ppwire = wire;
    *porder = order;
}

Topo_Edge* Topo_Face::GetFirstEdge()
{
    m_edges_it = m_edges.begin();
    return GetNextEdge();
}

Topo_Edge* Topo_Face::GetNextEdge()
{
    if(m_edges_it != m_edges.end())
        return (*m_edges_it++).m_edge;
    return 0;
}

Topo_Wire* Topo_Face::Project(Topo_Wire *wire)
{
	//Default operation is to project the wire onto m_plane by flattening the Z coordinate
	return wire->Project(m_plane);
}

bool Topo_Face::Contains(Topo_Wire *wire)
{
	//TODO: should never get called, throw exception
	return false;	
}