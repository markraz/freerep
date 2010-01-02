//This code is an adaptation of Davyd Madeley GtkGLExt example 
//It is licensed under the BSD license. 
//Any modifications from the original work are
//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions

#include "Geom_Vec3.h"

#include <vector>

class Triangle;
class Edge;

class Point
{
public:
	size_t m_id;
	std::vector<size_t> m_triangles;
	std::vector<size_t> m_edges; 
};

class Edge
{
public:
	size_t m_id;
	std::vector<size_t> m_triangles;
	size_t m_points[2];	
};

class Triangle
{
public:
	size_t m_edges[3];
	size_t m_points[3];
	size_t m_id;	
};

std::vector<Edge> nedges;
std::vector<Triangle> ntriangles;

Edge *MakeEdge()
{
	nedges.push_back(Edge());
	Edge *edge = &nedges[nedges.size()-1];
	edge->m_id = nedges.size()-1;
	return edge;	
}

Triangle *MakeTriangle()
{
	ntriangles.push_back(Triangle());
	Triangle *t = &ntriangles[ntriangles.size()-1];
	t->m_id = ntriangles.size()-1;
	return t;
}

void MaximumEdgeLength(const std::vector<Geom_Vec3> &pnts, const std::vector<int[3]> &triangles)
{
	nedges.clear();
	ntriangles.clear();
	
	//extend the points array with more per point info
	Point *points = new Point[pnts.size()];
		
	for(size_t i=0; i < triangles.size(); i++)
	{
		Triangle *t = MakeTriangle();
		Point *p1 = &points[triangles[i][0]];
		Point *p2 = &points[triangles[i][1]];
		Point *p3 = &points[triangles[i][2]];
		
		p1->m_triangles.push_back(t->m_id);
		p2->m_triangles.push_back(t->m_id);
		p3->m_triangles.push_back(t->m_id);
		
		Edge* e1 = 0;
		Edge* e2 = 0;
		Edge* e3 = 0;
		
		
		if(p1->m_triangles.size() > 1)
		{
			if(&points[nedges[p1->m_edges[0]].m_points[0]] == p2 || &points[nedges[p1->m_edges[0]].m_points[1]] == p2)
			{
				//triangle shares edge 1	
				e1 = &nedges[p1->m_edges[0]];
			}
			else if(&points[nedges[p1->m_edges[0]].m_points[0]] == p3 || &points[nedges[p1->m_edges[0]].m_points[1]] == p3)
			{	
				e3 = &nedges[p1->m_edges[0]];
			}
		}	
		
		if(p2->m_triangles.size() > 1)
		{
			if(&points[nedges[p2->m_edges[0]].m_points[0]] == p1 || &points[nedges[p2->m_edges[0]].m_points[1]] == p1)
			{
				//triangle shares edge 1	
				e1 = &nedges[p1->m_edges[0]];
			}
			else if(&points[nedges[p2->m_edges[0]].m_points[0]] == p3 || &points[nedges[p2->m_edges[0]].m_points[1]] == p3)
			{	
				e2 = &nedges[p1->m_edges[0]];
			}
		}
		
		if(p3->m_triangles.size() > 1)
		{
			if(&points[nedges[p3->m_edges[0]].m_points[0]] == p1 || &points[nedges[p3->m_edges[0]].m_points[1]] == p1)
			{
				e3 = &nedges[p1->m_edges[0]];
			}
			else if(&points[nedges[p3->m_edges[0]].m_points[0]] == p2 || &points[nedges[p3->m_edges[0]].m_points[1]] == p2)
			{	
				e2 = &nedges[p1->m_edges[0]];
			}
		}
		
		if(!e1)
			e1 = MakeEdge();
		if(!e2)
			e2 = MakeEdge();
		if(!e3)
			e3 = MakeEdge();
		
		p1->m_edges.push_back(e1->m_id);
		p1->m_edges.push_back(e3->m_id);
		
		p2->m_edges.push_back(e1->m_id);
		p2->m_edges.push_back(e2->m_id);
		
		p3->m_edges.push_back(e2->m_id);
		p3->m_edges.push_back(e3->m_id);
		
		e1->m_points[0] = triangles[i][0];
		e1->m_points[1] = triangles[i][1];
		e1->m_triangles.push_back(t->m_id);
		
		e2->m_points[0] = triangles[i][1];
		e2->m_points[1] = triangles[i][2];
		e2->m_triangles.push_back(t->m_id);
		
		e3->m_points[0] = triangles[i][2];
		e3->m_points[1] = triangles[i][0];
		e3->m_triangles.push_back(t->m_id);
		
		t->m_edges[0] = e1->m_id;
		t->m_edges[1] = e2->m_id;
		t->m_edges[2] = e3->m_id;
		
		t->m_points[0] = triangles[i][0];
		t->m_points[1] = triangles[i][1];
		t->m_points[2] = triangles[i][2];
	}
}