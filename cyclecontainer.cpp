#include "cyclecontainer.hpp"

//##################################################//
//													//
//					CycleContainer					//
//													//
//##################################################//

bool CycleContainer::alreadyInCycles(std::vector<int> cmp) const
{
	for(int i = 0; i < _cycles.size(); ++i)
	{
		if (_cycles[i]._path == cmp)
		{
			return true;
		}
	}
	return false;
}

bool CycleContainer::alreadyInSelfCycles(int cmp) const
{
	for (int i = 0; i < _selfcycles.size(); ++i)
	{
		if (_selfcycles[i] == cmp)
		{
			return true;
		}
	}
	return false;
}

int CycleContainer::getCycleAmount() const
{
	return _cycles.size();
}

int CycleContainer::getSelfCycleAmount() const
{
	return _selfcycles.size();
}

void CycleContainer::addCycle(std::vector<int> ncycle)
{
	if (ncycle.size() == 0)
	{
		return;
	}
	if (!alreadyInCycles(ncycle))
	{
		Cycle toadd;
		toadd._path = ncycle;
		_cycles.push_back(toadd);
	}
}

void CycleContainer::addSelfCycle(int nselfcycle)
{
	if (!alreadyInSelfCycles(nselfcycle))
	{
		_selfcycles.push_back(nselfcycle);
	}
}

/*
std::string CycleContainer::cycleToString(std::vector<int> vec) const
{
	std::string ret;
	//std::vector<int> c = rebuildCycle(_endcycle[index]);
	for (int i = 0; i < vec.size(); ++i)
	{
		ret += std::to_string(vec[i]) + (i != (vec.size()-1) ? " -> " : "\n");
	}

	return ret;
}
*/

std::string CycleContainer::cycleToString(int index) const
{
	std::string ret;
	if (index >= _cycles.size())
	{
		throw std::logic_error("CycleContainer: requested index higher than exists");
	}

	std::vector<int> c = _cycles[index]._path;
	for (int i = 0; i < c.size(); ++i)
	{
		ret += std::to_string(c[i]) + (i != (c.size()-1) ? " -> " : "\n");
	}

	return ret;
}


std::string CycleContainer::getInfo() const
{
	if (_filterparams.size() != _filtered.size())
	{
		throw std::logic_error("CycleContainer: There was a problem with filters");
	}

	std::string ret = "";

	ret += "### CYCLE CONTAINER INFO ###\n\n";
	int totalsize = _cycles.size() + _selfcycles.size();
	int nofiltersize = totalsize;

	for (int i = 0; i < _filtered.size(); ++i)
	{
		totalsize += _filtered[i].size();
	}

	ret += "Filters amount: " + std::to_string(_filterparams.size()) + "\n";
	ret += "Total cycles amount: " + std::to_string(totalsize) + "\n";
	ret += "From those " + std::to_string(totalsize) + " cycles:\n";
	ret += "\t " + std::to_string(_selfcycles.size()) + " loop into themselves\n";
	ret += "\t " + std::to_string(_cycles.size()) + " are normal cycles\n";

	if (_filterparams.size() != 0)
	{
		ret += "\t " + std::to_string(totalsize - nofiltersize) + " are filtered\n\n";
		ret += "Filter parameters are:\n";

		for (int i = 0; i < _filterparams.size(); ++i)
		{
			ret += "\t \'" + _filterparams[i] + "\': " + std::to_string(_filtered[i].size()) + " cycles\n";
		}
	}

	return ret;
}

/*
void CycleContainer::setGraph(Graph ref)
{
	_source = ref;
}
*/

/*
Graph CycleContainer::getGraph() const
{
	return _source;
}

*/

void CycleContainer::addFilteredCycle(int index, std::vector<int> toAdd)
{
	if (_filtered.size() <= index)
	{
		throw std::logic_error("CycleContainer: Tried to add to non-existant filter index");
	}

	_filtered[index].push_back(Cycle(toAdd));
}

void CycleContainer::addFilter(std::string nfilter)
{
	_filterparams.push_back(nfilter);
	_filtered.push_back(std::vector<Cycle>());
}

int CycleContainer::getFilteredCycleAmount() const
{
	return _filtered.size();
}

int CycleContainer::getFilteredCycleAmountAt(int index) const
{
	if (_filtered.size() <= index)
	{
		throw std::logic_error("CycleContainer: Tried to get non-existant filter index");
	}

	return _filtered[index].size();
}

std::string CycleContainer::getFilterName(int index) const
{
	if (_filterparams.size() <= index)
	{
		throw std::logic_error("CycleContainer: Tried to get non-existant filter index");
	}

	return _filterparams[index];
}

int CycleContainer::findFilter(std::string cmp) const
{
	int ret = -1;
	for (int i = 0; i < _filterparams.size(); ++i)
	{
		if (cmp == _filterparams[i])
		{
			ret = i;
			break;
		}
	}
	return ret;
}

bool CycleContainer::filterCheck(Cycle path, std::string filter, Graph reference)
{
	//bool ret = false;

	for (int i = 0; i < path._path.size(); ++i)
	{
		std::string pkgname = reference[path._path[i]].getName();
		if (pkgname.find(filter) != std::string::npos)
		{
			return true;
		}
		
		/*
		std::vector<std::string> reqs = reference[path._path[i]].getRequires();
		std::vector<std::string> provs = reference[path._path[i]].getProvides();

		for (int j = 0; j < reqs.size(); ++j)
		{
			if (reqs[j].find(filter) != std::string::npos)
			{
				return true;
			}
		}
		for (int j = 0; j < provs.size(); ++j)
		{
			if (provs[j].find(filter) != std::string::npos)
			{
				return true;
			}
		}
		*/
	}

	return false;
}

void CycleContainer::applyFilter(Graph reference)
{
	if (_filtered.size() != _filterparams.size())
	{
		throw std::logic_error("CycleContainer: filter sizes do not match (broken addFilter?)");
	}

	if (_filtered.size() == 0)
	{
		return;
	}

	std::vector<Cycle> newCycles;

	for (int i = 0; i < _cycles.size(); ++i)
	{
		std::cout << i << " " << _cycles.size() << ":";// << std::endl;
		std::vector<int> filterindex;
		for (int j = 0; j < _filterparams.size(); ++j)
		{
			if (filterCheck(_cycles[i], _filterparams[j], reference))
			{
				filterindex.push_back(j);
			}
		}

		if (filterindex.size() != 0)
		{
			std::cout << "Filtered for: ";
			for (int j = 0; j < filterindex.size(); ++j)
			{
				std::cout << "<" << _filterparams[filterindex[j]] << ">, ";
				_filtered[filterindex[j]].push_back(_cycles[i]);
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << "Not filtered" << std::endl;
			newCycles.push_back(_cycles[i]);
		}
	}
}

void CycleContainer::loadData(CycleSearcher resourse)
{
	std::vector<int> selfcycle = resourse.getSelfCycles();

	for (int i = 0; i < selfcycle.size(); ++i)
	{
		addSelfCycle(selfcycle[i]);
	}

	std::vector< std::vector<int> > cyclepath = resourse.getCycles();

	for (int i = 0; i < cyclepath.size(); ++i)
	{
		addCycle(cyclepath[i]);
		//std::cout << i << " out of " << _endcycle.size() << std::endl;
	}
}

std::vector<int> CycleContainer::getSelfCycle() const
{
	return _selfcycles;
}

std::vector< std::vector<int> > CycleContainer::getCycles() const
{
	std::vector< std::vector<int> > ret;

	for (int i = 0; i < _cycles.size(); ++i)
	{
		ret.push_back(_cycles[i]._path);
	}

	return ret;
}

std::vector< std::vector<int> > CycleContainer::getFilteredCycle(int index) const
{
	if ((index < 0) ||
		(index > _filtered.size())
	{
		throw std::logic_error("CycleContainer: Requested invalid filter index");
	}

	std::vector< std::vector<int> > ret;

	for (int i = 0; i < _filtered[index].size(); ++i)
	{
		ret.push_back(_filtered[index][i]._path);
	}

	return ret;
}

std::string CycleContainer::getFilterParameter(int index) const
{
	if ((index < 0) ||
		(index > _filterparams.size())
	{
		throw std::logic_error("CycleContainer: Requested invalid filter index");
	}

	return _filterparams[index];
}