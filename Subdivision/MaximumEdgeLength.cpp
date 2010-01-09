//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//MaximumEdgeLength.cpp - Author Jon Pry 

#include "Geom_Vec3.h"

#include <vector>
#include <list>

class Triangle;
class Edge;

class Point
{
public:
	size_t m_id;
	Geom_Vec3 m_loc;
	bool m_hastriangle;
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
std::vector<Point> npoints;

Point *MakePoint(Geom_Vec3 loc)
{
	npoints.push_back(Point());
	Point* point = &npoints[npoints.size()-1];
	point->m_loc = loc;	
	point->m_id = npoints.size()-1;
	point->m_hastriangle = false;
	return point;
}

Edge *MakeEdge()
{
	nedges.push_back(Edge());
	Edge *edge = &nedges[nedges.size()-1];
	edge->m_id = nedges.size()-1;
	return edge;	
}

double EdgeLength(size_t nedge)
{
	Geom_Vec3 p1 = npoints[nedges[nedge].m_points[0]].m_loc;	
	Geom_Vec3 p2 = npoints[nedges[nedge].m_points[1]].m_loc;
	return (p2 - p1).Norm();
}

Triangle *MakeTriangle()
{
	ntriangles.push_back(Triangle());
	Triangle *t = &ntriangles[ntriangles.size()-1];
	t->m_id = ntriangles.size()-1;
	return t;
}

//Here we build up a more complete data structure of the triangles
//We want to be able to pick a point and find the triangles and edges, 
//pick an edge and find points and triangles,
//pick a triangle and find points and edges

void BuildTris(const std::vector<Geom_Vec3> &pnts, const std::vector<int[3]> &triangles)
{
	nedges.clear();
	ntriangles.clear();
	npoints.clear();
	
	for(size_t i=0; i < pnts.size(); i++)
	{
		MakePoint(pnts[i]);	
	}
	
	for(size_t i=0; i < triangles.size(); i++)
	{
		Triangle *t = MakeTriangle();
		Point *p1 = &npoints[triangles[i][0]];
		Point *p2 = &npoints[triangles[i][1]];
		Point *p3 = &npoints[triangles[i][2]];

#if foo		
		p1->m_triangles.push_back(t->m_id);
		p2->m_triangles.push_back(t->m_id);
		p3->m_triangles.push_back(t->m_id);
#endif
		
		Edge* e1 = 0;
		Edge* e2 = 0;
		Edge* e3 = 0;
		
		//If this point has already been used, then there could be
		//a shared edge with another triangle
		//We can find it in constant time
		if(p1->m_hastriangle)
		{
			if(&npoints[nedges[p1->m_edges[0]].m_points[0]] == p2 || &npoints[nedges[p1->m_edges[0]].m_points[1]] == p2)
			{
				//triangle shares edge 1	
				e1 = &nedges[p1->m_edges[0]];
			}
			else if(&npoints[nedges[p1->m_edges[0]].m_points[0]] == p3 || &npoints[nedges[p1->m_edges[0]].m_points[1]] == p3)
			{	
				e3 = &nedges[p1->m_edges[0]];
			}
		}	
		
		if(p2->m_hastriangle)
		{
			if(&npoints[nedges[p2->m_edges[0]].m_points[0]] == p1 || &npoints[nedges[p2->m_edges[0]].m_points[1]] == p1)
			{
				//triangle shares edge 1	
				e1 = &nedges[p1->m_edges[0]];
			}
			else if(&npoints[nedges[p2->m_edges[0]].m_points[0]] == p3 || &npoints[nedges[p2->m_edges[0]].m_points[1]] == p3)
			{	
				e2 = &nedges[p1->m_edges[0]];
			}
		}
		
		if(p3->m_hastriangle)
		{
			if(&npoints[nedges[p3->m_edges[0]].m_points[0]] == p1 || &npoints[nedges[p3->m_edges[0]].m_points[1]] == p1)
			{
				e3 = &nedges[p1->m_edges[0]];
			}
			else if(&npoints[nedges[p3->m_edges[0]].m_points[0]] == p2 || &npoints[nedges[p3->m_edges[0]].m_points[1]] == p2)
			{	
				e2 = &nedges[p1->m_edges[0]];
			}
		}
		
		//Mark the points as used
		p1->m_hastriangle = true;
		p2->m_hastriangle = true;
		p3->m_hastriangle = true;
		
		//Make edges that have not been located
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

//Split an edge, creating at most 2 new triangles. 
size_t SplitEdge(size_t nedge)
{
	Geom_Vec3 p1 = npoints[nedges[nedge].m_points[0]].m_loc;	
	Geom_Vec3 p2 = npoints[nedges[nedge].m_points[1]].m_loc;
	Geom_Vec3 mpt = ((p2 - p1) / 2) + p1;
	Point *npt = MakePoint(mpt);
	npt->m_hastriangle = true;
	
	Edge* tedge = &nedges[nedge];
	
	Triangle *t1 = &ntriangles[tedge->m_triangles[0]];
	Triangle *t2 = 0;
	if(tedge->m_triangles.size() > 1)
		t2 = &ntriangles[tedge->m_triangles[1]];
		
	Point* t1opnt=0;
	if(t1->m_edges[0] != nedge)
	{
		if(nedges[t1->m_edges[0]].m_points[0] != tedge->m_points[0] &&	
				nedges[t1->m_edges[0]].m_points[0] != tedge->m_points[1])
			t1opnt = &npoints[nedges[t1->m_edges[0]].m_points[0]];		
		else
			t1opnt = &npoints[nedges[t1->m_edges[0]].m_points[1]];
	}
	else
	{
		if(nedges[t1->m_edges[1]].m_points[0] != tedge->m_points[0] &&	
				nedges[t1->m_edges[1]].m_points[0] != tedge->m_points[1])
		{
			t1opnt = &npoints[nedges[t1->m_edges[1]].m_points[0]];			
		}
		else
		{
			t1opnt = &npoints[nedges[t1->m_edges[1]].m_points[1]];
		} 
	}
	
	Point* t2opnt=0;
	if(t2)
	{
		if(t2->m_edges[0] != nedge)
		{
			if(nedges[t2->m_edges[0]].m_points[0] != tedge->m_points[0] &&	
					nedges[t2->m_edges[0]].m_points[0] != tedge->m_points[1])
				t2opnt = &npoints[nedges[t2->m_edges[0]].m_points[0]];		
			else
				t2opnt = &npoints[nedges[t2->m_edges[0]].m_points[1]];
		}
		else
		{
			if(nedges[t2->m_edges[1]].m_points[0] != tedge->m_points[0] &&	
					nedges[t2->m_edges[1]].m_points[0] != tedge->m_points[1])
			{
				t2opnt = &npoints[nedges[t2->m_edges[1]].m_points[0]];			
			}
			else
			{
				t2opnt = &npoints[nedges[t2->m_edges[1]].m_points[1]];
			} 
		}
	} 	
	
	Triangle *t1new = MakeTriangle();
	Triangle *t2new = 0;
	if(t2)	
		t2new = MakeTriangle();
	 	
	Edge* newedge = MakeEdge();
	Point* lpt = &npoints[tedge->m_points[0]];
	Point* rpt = &npoints[tedge->m_points[1]];
	
	Edge* t1nedge = MakeEdge();
	t1nedge->m_triangles.push_back(t1->m_id);
	t1nedge->m_triangles.push_back(t1new->m_id);
	t1nedge->m_points[0] = t1opnt->m_id;
	t1nedge->m_points[1] = npt->m_id;
	
	Edge* t2nedge = 0;
	if(t2)
	{
		t2nedge = MakeEdge();
		t2nedge->m_triangles.push_back(t2->m_id);
		t2nedge->m_triangles.push_back(t2new->m_id);
		t2nedge->m_points[0] = t2opnt->m_id;
		t2nedge->m_points[1] = npt->m_id;
	}
	
	newedge->m_points[0] = tedge->m_points[1];
	tedge->m_points[1] = npt->m_id;
	newedge->m_points[0] = npt->m_id;
	
	
}

//After getting our complete data structure, we make a list of all
//the triangles and then loop the list until it is empty
//we check each triangle to see if it has an overlength edge
//if so we split it, and keep it in the list for further iterations
//when done, ntriangles will have only tris with edge length less
//than the minimum

void MaximumEdgeLength(double tol, const std::vector<Geom_Vec3> &pnts, const std::vector<int[3]> &tris)
{
	BuildTris(pnts,tris);
	
	std::list<size_t> possibles;
	for(size_t i = 0; i < ntriangles.size(); i++)
		possibles.push_back(i);
		
	while(possibles.size())
	{
		std::list<size_t>::iterator it;
		for(it = possibles.begin(); it != possibles.end(); it++)
		{
			std::list<size_t>::iterator it2 = it++;
			
			if(EdgeLength(ntriangles[*it2].m_edges[0]) > tol)
			{
				SplitEdge(ntriangles[*it2].m_edges[0]);
			}
			else if(EdgeLength(ntriangles[*it2].m_edges[1]) > tol)
				possibles.push_back(SplitEdge(ntriangles[*it2].m_edges[1]));
			else if(EdgeLength(ntriangles[*it2].m_edges[2]) > tol)
				possibles.push_back(SplitEdge(ntriangles[*it2].m_edges[2]));
			else 
				possibles.erase(it);
		}	
	}
 }