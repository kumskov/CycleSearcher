#include "cyclesearcher.hpp"

CycleSearcher::CycleSearcher() : Graph()
{
}

bool CycleSearcher::allVisited() const
{
	bool ret = true;
	for (int i = 0; i < _visited.size(); ++i)
	{
		if (_visited[i]._mark == unvisitedMark)
		{
			//std::cout << i << " unvisited" << std::endl;
			ret = false;
		}
	}

	return ret;
}

int CycleSearcher::findFirstUnvisited() const
{
	for (int i = 0; i < _visited.size(); ++i)
	{
		if (_visited[i]._mark == unvisitedMark)
		{
			//std::cout << i << ' ' << _visited[i]._mark << std::endl;
			return i;
		}
	}

	return -1;
}

bool CycleSearcher::addSelfCyclingNode(int index)
{
	bool alreadyAdded = false;
	for (int i = 0; i < _selfcycle.size(); ++i)
	{
		if (index == _selfcycle[i])
		{
			alreadyAdded = true;
			break;
		}
	}

	if (!alreadyAdded)
	{
		_selfcycle.push_back(index);
		return true;
	}

	return false;
}

void CycleSearcher::checkSelfLinks()
{
	//int ret = false;
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		for (int j = 0; j < _ingraph[i]._requires.size(); ++j)
		{
			if (i == _ingraph[i]._requires[j])
			{
				//std::cout << _ingraph[i]._pkg.getFullInfo() << std::endl;
				_visited[i]._mark = selfCycleMark;
				_visited[i]._loopIndex = i;
				addSelfCyclingNode(i);
				//ret = true;
			}
		}

		for (int j = 0; j < _ingraph[i]._providesFor.size(); ++j)
		{
			if (i == _ingraph[i]._providesFor[j])
			{
				_visited[i]._mark = selfCycleMark;
				_visited[i]._loopIndex = i;
				addSelfCyclingNode(i);
				//std::cout << _ingraph[i]._pkg.getFullInfo() << std::endl;
				//ret = true;
			}
		}
	}

	//return ret;
}

void CycleSearcher::findCycles()
{
	if (_ingraph.size() == 0)
		throw std::logic_error("CycleSearcher: Need to build graph first!");


	for (int i = 0; i < _ingraph.size(); ++i)
	{
		FireHistory elem;
		elem._mark = unvisitedMark;
		elem._loopIndex = -1;
		_visited.push_back(elem);
	}

	checkSelfLinks();

	int toStart = findFirstUnvisited();
	int mark = starterMark;;
	while(toStart != unvisitedMark)
	{
		std::vector<int> whatever;
		fire(toStart, mark, std::vector<int>());
		mark++;
		toStart = findFirstUnvisited();
		//std::cout << mark << std::endl;
	}


	std::cout << "Finished finding cycles" << std::endl;

	std::cout << _endcycle.size() << " cycles found" << std::endl;
	std::cout << _selfcycle.size() << " selfcycles found" << std::endl;

	std::cout << "Did we visit all nodes? " << (allVisited() ? "YES" : "NO") << std::endl;
}

std::string CycleSearcher::cycleToString(int index) const
{
	std::string ret;
	std::vector<int> c = rebuildCycle(_endcycle[index]);
	for (int i = 0; i < c.size(); ++i)
	{
		ret += std::to_string(c[i]) + (i != (c.size()-1) ? " -> " : "\n");
	}

	return ret;
}

int CycleSearcher::cycleAmount() const
{
	return _endcycle.size();
}

int CycleSearcher::selfCycleAmount() const
{
	return _selfcycle.size();
}

void CycleSearcher::fire(int index, int mark, std::vector<int> path)
{
	_visited[index]._mark = mark;
	_visited[index]._path = path;

	for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	{
		if (_visited[_ingraph[index]._providesFor[i]]._mark == mark)
		{
			//std::cout << _endcycle.size() << " cycle ends with node " << index << std::endl;
			_endcycle.push_back(index);
			_visited[index]._loopIndex = _ingraph[index]._providesFor[i];
			//return;
		}
		else
		{
			std::vector<int> npath = path;
			npath.push_back(index);
			fire(_ingraph[index]._providesFor[i], mark, npath);
		}
	}
}

std::vector<int> CycleSearcher::rebuildCycle(int index) const
{

	std::vector<int> ret = _visited[index]._path;
	ret.push_back(index);

	int loopHead = -1;
	for (int i = 0; i < ret.size(); ++i)
	{
		if (ret[i] == _visited[index]._loopIndex)
		{
			loopHead = i;
			break;
		}
	}

	ret.push_back(_visited[index]._loopIndex);
	if (loopHead != -1)
	{
		//ret.push_back(_visited[index]._loopIndex);
	}
	else
	{
		std::cout << "Error recreating loop, may be broken" << std::endl;
		//throw std::logic_error("CAN'T WAKE UP");
		return ret;
	}

	//std::vector<int> ret(tmp.begin() + loopHead, tmp.end());

/*
	for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	{
		int stop = false;
		for (int j = 0; j < _visited[index]._path.size(); ++j)
		{
			if (_ingraph[index]._providesFor[i] == _visited[index]._path[j])
			{
				stop = true;
				ret.push_back(_ingraph[index]._providesFor[i]);
				break;
			}
		}

		if (stop)
		{
			break;
		}

	}
*/
	for (int i = 0; i < ret.size(); ++i)
	{
		if (ret[i] == ret[ret.size() - 1])
		{
			ret = std::vector<int>(ret.begin()+i, ret.end());
			break;
		}
	}


	return ret;
}