#ifndef __CYCLESEARCHER_HPP__
#define __CYCCLESEARCHER_HPP__

#include "graph.hpp"

class CycleSearcher : public Graph
{
protected:
	struct FireHistory
	{
		int _treenum;
		std::vector<int> _path;
	};

	std::vector<FireHistory> _visited;
	//Every time a node "ends" a cycle, i.e. links to a node
	//that is already visited, it's added here in order to be able to
	//reconstruct the cycle
	std::vector<int> _endcycle;
	//std::vector< std::vector<int> > _cycles;

	bool allVisited() const;

	//"Fire in the praire" algorithm
	//Basicly the same as dijkstra but insted of marking nodes
	//as "visited/not visited" we place length of recurse
	//Resursive, starts with 0
	void fire(int, int, std::vector<int>);

	int findFirstUnvisited() const;

public:
	CycleSearcher();
	//Build a cycle from "ending" nodes
	std::vector<int> rebuildCycle(int) const;

	int cycleAmount() const;

	void findCycles();

	std::string cycleToString(int) const;
};


#endif