#include "cyclesearcher.hpp"

CycleSearcher::CycleSearcher() : Graph()
{
}

bool CycleSearcher::allVisited() const
{
	bool ret = true;
	for (int i = 0; i < _visited.size(); ++i)
	{
		if (_visited[i]._treenum == -1)
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
		if (_visited[i]._treenum == -1)
		{
			//std::cout << i << ' ' << _visited[i]._treenum << std::endl;
			return i;
		}
	}

	return -1;
}


void CycleSearcher::findCycles()
{
	if (_ingraph.size() == 0)
		throw std::logic_error("CycleSearcher: Need to build graph first!");

	for (int i = 0; i < _ingraph.size(); ++i)
	{
		FireHistory elem;
		elem._treenum = -1;
		_visited.push_back(elem);
	}

	int toStart = findFirstUnvisited();
	int treeNum = 0;
	while(toStart != -1)
	{
		std::vector<int> whatever;
		fire(toStart, treeNum, std::vector<int>());
		treeNum++;
		toStart = findFirstUnvisited();
		//std::cout << treeNum << std::endl;
	}

	//std::cout << "Finished finding cycles" << std::endl;

	//std::cout << _endcycle.size() << " cycles found" << std::endl;

	//std::cout << "Did we visit all nodes? " << (allVisited() ? "YES" : "NO") << std::endl;

	
}

std::string CycleSearcher::cycleToString(int index) const
{
	std::string ret;
	std::vector<int> c = rebuildCycle(_endcycle[index]);
	for (int i = 0; i < c.size(); ++i)
	{
		ret += std::to_string(c[i]) + (i != (c.size()-1) ? " <- " : "\n");
	}

	return ret;
}

int CycleSearcher::cycleAmount() const
{
	return _endcycle.size();
}

void CycleSearcher::fire(int index, int mark, std::vector<int> path)
{
	_visited[index]._treenum = mark;
	_visited[index]._path = path;

	for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	{
		if (_visited[_ingraph[index]._providesFor[i]]._treenum == mark)
		{
			//std::cout << _endcycle.size() << " cycle ends with node " << index << std::endl;
			_endcycle.push_back(index);
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

		/*
		int elem = _ingraph[index]._providesFor[i];
		std::vector<int>::iterator it = 				\
			std::find(_visited[index]._path.begin(), 	\
					_visited[index]._path.end(), 		\
					);

		if (it != _visited[index]._path.end())
		{
			ret.push_back(_ingraph[index]._providesFor[i]);
			break;
		}	
		*/	
	}

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