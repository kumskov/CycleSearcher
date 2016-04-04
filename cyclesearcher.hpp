#ifndef __CYCLESEARCHER_HPP__
#define __CYCCLESEARCHER_HPP__

#include "graph.hpp"

class CycleSearcher : public graph
{
protected:
	std::vector<int> _visited;
	//Every time a node "ends" a cycle, i.e. links to a node
	//that is already visited, it's added here in order to be able to
	//reconstruct the cycle
	std::vector<int> _endcycle;

	bool allVisited() const;
public:
	

};


#endif