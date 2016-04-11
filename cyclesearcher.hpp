#ifndef __CYCLESEARCHER_HPP__
#define __CYCCLESEARCHER_HPP__

#include "graph.hpp"


class CycleContainer
{
protected:
	struct Cycle
	{
		std::vector<int> _path;
	};

	std::vector<Cycle> _cycles;
	std::vector<int> _selfcycles;

	bool alreadyAdded(std::vector<int>) const;
	bool alreadyInSelfCycles(int) const;

public:
	CycleContainer() {};

	int getCycleAmount() const;
	int getSelfCycleAmount() const;
	void addCycle(std::vector<int>);
	void addSelfCycle(int);

	std::string getInfo() const;

	//std::string cycleToString(std::vector<int>) const;
	std::string cycleToString(int) const;
};



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


	std::vector< std::vector<int> > _cyclepath;

	bool allVisited() const;

	//"Fire in the praire" algorithm
	//Basicly the same as dijkstra but insted of marking nodes
	//as "visited/not visited" we place length of recurse
	//Resursive, starts with 0
	void fire(int, int, std::vector<int>);

	int findFirstUnvisited() const;

	void checkSelfLinks();
	//void countSlots() const;

	//return true if added
	bool addSelfCyclingNode(int);

	std::vector<int> rebuildCycle(int) const;

	void addEndCycle(int);
	void addEndCycle(std::vector<int>);

	bool isCycle(std::vector<int>) const;

public:
	CycleSearcher();
	//Build a cycle from "ending" nodes
	CycleContainer getCycles() const;
	std::string cycleToString(int) const;

	//int cycleAmount() const;
	//int selfCycleAmount() const;

	void findCycles();


};


#endif