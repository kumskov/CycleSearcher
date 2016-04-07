#ifndef __CYCLESEARCHER_HPP__
#define __CYCCLESEARCHER_HPP__

#include "graph.hpp"

class CycleSearcher : public Graph
{
protected:
	const int selfCycleMark = 0;
	const int unvisitedMark = -1;
	const int starterMark = 1;

	struct FireHistory
	{
		int _mark;
		std::vector<int> _path;
		int _loopIndex;
	};

	std::vector<FireHistory> _visited;
	//Every time a node "ends" a cycle, i.e. links to a node
	//that is already visited, it's added here in order to be able to
	//reconstruct the cycle
	std::vector<int> _selfcycle;
	std::vector<int> _endcycle;
	//std::vector< std::vector<int> > _cycles;

	bool allVisited() const;

	//"Fire in the praire" algorithm
	//Basicly the same as dijkstra but insted of marking nodes
	//as "visited/not visited" we place length of recurse
	//Resursive, starts with 0
	void fire(int, int, std::vector<int>);

	int findFirstUnvisited() const;

	void checkSelfLinks();

	//return true if added
	bool addSelfCyclingNode(int);

public:
	CycleSearcher();
	//Build a cycle from "ending" nodes
	std::vector<int> rebuildCycle(int) const;

	int cycleAmount() const;
	int selfCycleAmount() const;

	void findCycles();

	std::string cycleToString(int) const;
};


#endif