#ifndef __CYCLECONTAINER_HPP__
#define __CYCLECONTAINER_HPP__

#include "package.hpp"
#include "graph.hpp"
#include "cyclesearcher.hpp"

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

	//Graph _source;

	bool alreadyInCycles(std::vector<int>) const;
	bool alreadyInSelfCycles(int) const;

	bool filterCheck(Cycle, std::string, Graph);

public:
	CycleContainer() {};

	void loadData(CycleSearcher);

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

	//void setGraph(Graph);
	//Graph getGraph() const;

	std::string getInfo() const;

	//std::string cycleToString(std::vector<int>) const;
	std::string cycleToString(int) const;

	void applyFilter(Graph);

	std::vector<int> getSelfCycles() const;
	std::vector< std::vector<int> > getCycles() const;

	std::vector< std::vector<int> > getFilteredCycle(int) const;
	std::string getFilterParameter(int) const;

	int getFilterAmount() const;
};



#endif