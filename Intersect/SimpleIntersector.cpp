//Copyright (C) 2010 The FreeREP Foundation
//See License.txt for terms and conditions
//SimpleIntersector.cpp - Author Jon Pry

#include "SimpleIntersector.h"

#include <set>

Topo_Wire *MergeWires(std::set<Topo_Wire*> &wires)
{
	std::set<Topo_Wire*>::iterator it=wires.begin();
	Topo_Wire* wire = *it;
	for(it++;it!=wires.end();it++)
	{
		wire = wire->Merge(*it);	
	}
	
	return wire;
}

void SimpleIntersector::Intersect(std::vector<Topo_Wire*> &wires)
{
	std::vector<std::pair<Topo_Wire*,Topo_Wire*> > wires_to_merge;
	//First we deal with coincident lines
	for(size_t i=0; i < wires.size(); i++)
	{
		Topo_Wire *awire = wires[i];
		for(size_t j=i+1; j < wires.size(); j++)
		{
			Topo_Wire *bwire = wires[j];
			if(awire->IsCoincident(bwire))
			{
				wires_to_merge.push_back(std::pair<Topo_Wire*,Topo_Wire*>(awire,bwire));				
			}
		}
	}
	
	std::vector<Topo_Wire*> intersectable_wires;
	std::set<Topo_Wire*> already_merged;
	//Figure out the chains of merging, and merge them
	for(size_t i=0; i < wires_to_merge.size(); i++)
	{
		if(already_merged.find(wires_to_merge[i].first) == already_merged.end() ||
		already_merged.find(wires_to_merge[i].second) == already_merged.end())
			continue;
		
		std::set<Topo_Wire*> this_merge;
		this_merge.insert(wires_to_merge[i].first);
		this_merge.insert(wires_to_merge[i].second);
		size_t last_count = 0;
		//Keep looping through until there are no more chains to locate
		//TODO: this has fairly bad potential complexity, but for 
		//non pathological cases it should be pretty fast
	
		while(last_count != this_merge.size())
		{
			last_count = this_merge.size();
			for(size_t j=0; j < wires_to_merge.size(); j++)
			{
				if(this_merge.find(wires_to_merge[j].first) != this_merge.end())
				{
					if(this_merge.find(wires_to_merge[j].second) == this_merge.end())
						this_merge.insert(wires_to_merge[j].second);	
				}
				else if(this_merge.find(wires_to_merge[j].second) != this_merge.end())
				{
					if(this_merge.find(wires_to_merge[j].first) == this_merge.end())
						this_merge.insert(wires_to_merge[j].first);	
				}
			}			
		}
		
		intersectable_wires.push_back(MergeWires(this_merge));
		std::set<Topo_Wire*>::iterator it;
		for(it = this_merge.begin(); it != this_merge.end(); it++)
		{
			already_merged.insert(*it);	
		}
	}
	
	//Make a new list that doesn't have the wires that were merged in it
	for(size_t i=0; i < wires.size(); i++)
	{
		if(already_merged.find(wires[i]) == already_merged.end())
			intersectable_wires.push_back(wires[i]);
	}
	
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