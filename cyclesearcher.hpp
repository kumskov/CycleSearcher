#ifndef __CYCLESEARCHER_HPP__
#define __CYCLESEARCHER_HPP__

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
		//std::vector<int> _path;
		//int _loopIndex;
	};
	

	std::vector<FireHistory> _visited;
	//Every time a node "ends" a cycle, i.e. links to a node
	//that is already visited, it's added here in order to be able to
	//reconstruct the cycle
	std::vector<int> _selfcycle;
	//std::vector<int> _endcycle;
	//std::vector< std::vector<int> > _cycles;


	std::vector< std::vector<int> > _cyclepath;

	bool allVisited() const;

	//Basicly the same as dijkstra but insted of marking nodes
	//as "visited/not visited" we place length of recurse
	//Resursive, starts with 0
	void fire(int, int, std::vector<int>);

	int findFirstUnvisited() const;

	//Finds all selfcycling nodes
	void checkSelfLinks();
	//void countSlots() const;

	//return true if added
	bool addSelfCyclingNode(int);

	//std::vector<int> rebuildCycle(int) const;

	std::vector<int> extractCycle(std::vector<int>) const;

	//void addEndCycle(int);
	void addEndCycle(std::vector<int>);

	bool isCycle(std::vector<int>) const;

public:
	CycleSearcher();
	//Build a cycle from "ending" nodes
	//CycleContainer getCycleContainer() const;
	//std::string cycleToString(int) const;

	std::vector< std::vector<int> > getCycles() const;
	std::vector<int> getSelfCycles() const;

	//int cycleAmount() const;
	//int selfCycleAmount() const;

	void findCycles();


};


#endif