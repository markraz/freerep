//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//SimpleIntersector.cpp - Author Jon Pry

#include "SimpleIntersector.h"

void SimpleIntersector::Intersect(std::vector<Topo_Wire*> &wires)
{
	for(size_t i=0; i < wires.size(); i++)
	{
		Topo_Wire *awire = wires[i];
		for(size_t j=i+1; j < wires.size(); j++)
		{
			Topo_Wire *bwire = wires[j];
			
			if(awire->GetStart() == bwire->GetStart() || awire->GetStart() == bwire->GetEnd() ||
				awire->GetEnd() == bwire->GetStart() || awire->GetEnd() == bwire->GetEnd())
			{
				//We generally want to avoid these lines, but if they are coincident, we must still handle it
				//first check to see if they are the same line	
				
				if((awire->GetStart() == bwire->GetStart() && awire->GetEnd() == bwire->GetEnd()) ||
					(awire->GetStart() == bwire->GetEnd() && awire->GetEnd() == bwire->GetStart()))
				{
					//TODO: throw one of them away. there may be intersections calculated with both
					//of these, so we must be sure to throw the intersection involved with one away	
				}
				else
				{
					//TODO:check for coincidence
				}
			}
			
			Intersection ipnt = awire->Intersect(bwire);
			
			//TODO: figure out if lines are coincident, special handling required
		}
	}	
} 