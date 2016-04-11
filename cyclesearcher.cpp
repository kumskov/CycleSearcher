#include "cyclesearcher.hpp"

//##################################################//
//													//
//					CycleContainer					//
//													//
//##################################################//

bool CycleContainer::alreadyAdded(std::vector<int> cmp) const
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
	if (!alreadyAdded(ncycle))
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
	return "FIXME";
}

//##################################################//
//													//
//					CycleSearcher					//
//													//
//##################################################//

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
			for (int k = 0; k < _ingraph[i]._requires[j]._possibleMatch.size(); ++k)
			{
				if (i == _ingraph[i]._requires[j]._possibleMatch[k])
				{
					//std::cout << _ingraph[i]._pkg.getFullInfo() << std::endl;
					_visited[i]._mark = selfCycleMark;
					_visited[i]._loopIndex = i;
					addSelfCyclingNode(i);
					//ret = true;
				}
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

/*
void CycleSearcher::countSlots() const
{
	int cnt = 0;
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		for (int j = 0; j < _ingraph[i]._requires.size(); ++j)
		{
			if (_ingraph[i]._requires[j]._possibleMatch.size() > 1)
			{
				std::cout << i << ": slot possibility" << std::endl;
				cnt++;
				break;
			}
		}
	}
	std::cout << "Total slotted packages: " << cnt << std::endl;
}
*/

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
	//countSlots();
	int toStart = findFirstUnvisited();
	int mark = starterMark;;

	while(toStart != unvisitedMark)
	//for (int i = 0; i < _ingraph.size(); ++i)
	{
		//std::vector<int> whatever;
		fire(toStart, mark, std::vector<int>());
		//fire(i, mark, std::vector<int>());
		mark++;
		toStart = findFirstUnvisited();
		std::cout << "Finished " << mark << std::endl;
	}


	std::cout << "Finished finding cycles" << std::endl;

	//std::cout << _endcycle.size() << " cycles found" << std::endl;
	std::cout << _cyclepath.size() << " cycles found" << std::endl;
	std::cout << _selfcycle.size() << " selfcycles found" << std::endl;

	std::cout << "Did we visit all nodes? " << (allVisited() ? "YES" : "NO") << std::endl;

}

/*
int CycleSearcher::cycleAmount() const
{
	return _endcycle.size();
}

int CycleSearcher::selfCycleAmount() const
{
	return _selfcycle.size();
}
*/

void CycleSearcher::addEndCycle(int nval)
{
	//std::cout << "Add End Cycle" << std::endl;
	bool exists = false;
	for (int i = 0; i < _endcycle.size(); ++i)
	{
		if (_endcycle[i] == nval)
		{
			exists = true;
			//std::cout << "Repeated" << std::endl;
			break;
		}
	}
	if (!exists)
	{
		_endcycle.push_back(nval);
	}
}

void CycleSearcher::addEndCycle(std::vector<int> nend)
{
	//std::cout << "Add End Cycle" << std::endl;
	bool exists = false;
	for (int i = 0; i < _cyclepath.size(); ++i)
	{
		if (_cyclepath[i] == nend)
		{
			exists = true;
			//std::cout << "Repeated" << std::endl;
			break;
		}
	}
	if (!exists)
	{
		_cyclepath.push_back(nend);
		//std::cout << "Added " << _cyclepath.size() << std::endl;
	}
}

bool CycleSearcher::isCycle(std::vector<int> test) const
{
	for (int i = 0; i < test.size()-1; ++i)
	{
		if (test[i] == test[test.size() - 1])
		{
			return true;
		}
	}
	return false;
}

void CycleSearcher::fire(int index, int mark, std::vector<int> path)
{
	
	if (_visited[index]._mark == selfCycleMark)
	{
		return;
	}
	
	std::cout << "Mark " << mark << " index " << index << ": " << path.size() << std::endl;
	_visited[index]._mark = mark;
	_visited[index]._path = path;

	/*
	for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	{
		std::vector<int> npath = path;
		npath.push_back(index);
		if (isCycle(npath))
		{
			addEndCycle(npath);
			std::cout << "Exit recursion" << std::endl;
		}
		else
		{
			fire(_ingraph[index]._providesFor[i], mark, npath);	
		}
	}
	*/

	
	for (int i = 0; i < _ingraph[index]._requires.size(); ++i)
	{
		for (int j = 0; j < _ingraph[index]._requires[i]._possibleMatch.size(); ++j)
		{
			std::vector<int> npath = path;
			npath.push_back(index);
			if (isCycle(npath))
			{
				addEndCycle(npath);
				std::cout << "Exit" << std::endl;
			}
			else
			{
				fire(_ingraph[index]._requires[i]._possibleMatch[j], mark, npath);
			}
		}
	}
	

	//Old provides check
	/*
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
	*/
	
	//old requires check
	/*
	for (int i = 0; i < _ingraph[index]._requires.size(); ++i)
	{
		for (int j = 0; j < _ingraph[index]._requires[i]._possibleMatch.size(); ++j)
		{
			//if (_visited[_ingraph[index]._requires[i]._possibleMatch[j]]._mark < mark)
			//{
			//	continue;
			//}
			if (_visited[_ingraph[index]._requires[i]._possibleMatch[j]]._mark == mark)
			{
				//std::cout << _endcycle.size() << " cycle ends with node " << index << std::endl;
				//_endcycle.push_back(index);
				addEndCycle(index);
				_visited[index]._loopIndex = _ingraph[index]._requires[i]._possibleMatch[j];
				//return;
			}
			else
			{
				std::vector<int> npath = path;
				npath.push_back(index);
				fire(_ingraph[index]._requires[i]._possibleMatch[j], mark, npath);
			}
		}
	}
	*/

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
		std::string tmp;
		//std::cout << "Error recreating loop, may be broken" << std::endl;
		//for (int i = 0; i < ret.size(); ++i)
		//{
		//	tmp += std::to_string(ret[i]) + (i != (ret.size()-1) ? " -> " : "\n");
		//}
		//std::cout << tmp;
		//throw std::logic_error("CAN'T WAKE UP");
		

		//return std::vector<int>();

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

CycleContainer CycleSearcher::getCycles() const
{
	CycleContainer ret;

	for (int i = 0; i < _selfcycle.size(); ++i)
	{
		//ret.addSelfCycle(_selfcycle[i]);
	}

	for (int i = 0; i < _endcycle.size(); ++i)
	{
		//std::vector<int> toadd = rebuildCycle(_endcycle[i]);
		//ret.addCycle(toadd);
		//std::cout << i << " out of " << _endcycle.size() << std::endl;
	}

	return ret;
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
