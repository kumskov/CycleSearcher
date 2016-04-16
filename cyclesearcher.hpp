#ifndef __CYCLESEARCHER_HPP__
#define __CYCCLESEARCHER_HPP__

#include "graph.hpp"


class CycleContainer
{
protected:
	struct Cycle
	{
		std::vector<int> _path;

		Cycle(std::vector<int> src)
		{
			_path = src;
		}

		Cycle() {}
	};

	std::vector<Cycle> _cycles;
	std::vector<int> _selfcycles;

	std::vector< std::vector<Cycle> > _filtered;
	std::vector< std::string> _filterparams;

	Graph _source;

	bool alreadyInCycles(std::vector<int>) const;
	bool alreadyInSelfCycles(int) const;

public:
	CycleContainer() {};

	int getCycleAmount() const;
	int getSelfCycleAmount() const;
	int getFilteredCycleAmount() const;
	int getFilteredCycleAmountAt(int) const;
	std::string getFilterName(int) const;

	void addCycle(std::vector<int>);
	void addSelfCycle(int);
	void addFilteredCycle(int, std::vector<int>);
	void addFilter(std::string);

	int findFilter(std::string) const;

	void setGraph(Graph);
	Graph getGraph() const;

	std::string getInfo() const;

	//std::string cycleToString(std::vector<int>) const;
	std::string cycleToString(int) const;

	void applyFilter();
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
	CycleContainer getCycleContainer() const;
	//std::string cycleToString(int) const;

	//int cycleAmount() const;
	//int selfCycleAmount() const;

	void findCycles();


};


#endif