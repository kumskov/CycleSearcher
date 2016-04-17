#include "cyclesearcher.hpp"


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
					//_visited[i]._loopIndex = i;
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
				//_visited[i]._loopIndex = i;
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
		//elem._loopIndex = -1;
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
		//std::cout << "\n###############################" << std::endl;
		fire(toStart, mark, std::vector<int>());
		//fire(i, mark, std::vector<int>());
		mark++;
		toStart = findFirstUnvisited();
		//std::cout << "Finished " << mark << std::endl;
	}

	if (!allVisited())
	{
		throw std::logic_error("CycleSearcher: An error occured - did not check all nodes");
	}

	/*
	std::cout << "Finished finding cycles" << std::endl;

	std::cout << _endcycle.size() << " endcycles found" << std::endl;
	std::cout << _cyclepath.size() << " cyclepaths found" << std::endl;
	std::cout << _selfcycle.size() << " selfcycles found" << std::endl;

	std::cout << "Did we visit all nodes? " << (allVisited() ? "YES" : "NO") << std::endl;
	*/
	/*
	for (int i = 0; i < _cyclepath[0].size(); ++i)
	{
		std::cout << _cyclepath[0][i] << ' ';
	}
	std::cout << std::endl;
	*/
	/*
	for (int i = 0; i < _cyclepath.size(); ++i)
	{
		std::cout << i << ": " << (isCycle(_cyclepath[i]) ? "YES" : "NO") << std::endl;
	}
	*/
}

/*
int CycleSearcher::cycleAmount() const
{
	return _endcycle.size();
}
*/

/*
int CycleSearcher::selfCycleAmount() const
{
	return _selfcycle.size();
}
*/

/*
void CycleSearcher::addEndCycle(int nval)
{
	//std::cout << "Add End Cycle: ";
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
		//std::cout << "Added" << std::endl;
		_endcycle.push_back(nval);
	}
}
*/

void CycleSearcher::addEndCycle(std::vector<int> nend)
{
	/*
	for (int i = 0; i < nend.size(); ++i)
	{
		std::cout << nend[i] << ' ';
	}
	std::cout << std::endl;
	*/


	//std::cout << "Add End Cycle: ";
	bool exists = false;
	for (int i = 0; i < _cyclepath.size(); ++i)
	{
		if (_cyclepath[i].size() == nend.size())
		{
			bool cmp = true;
			for (int j = 0; j < _cyclepath[i].size(); ++j)
			{
				if (nend[j] != _cyclepath[i][j])
				{
					cmp = false;
					break;
				}
			}
			if (cmp)
			{
				exists = true;
				//std::cout << "repeated" << std::endl;
				break;
			}
		}
		//if (_cyclepath[i] == nend)
		//{
		//	exists = true;
		//	std::cout << "Repeated" << std::endl;
		//	break;
		//}
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
		//std::cout << "selfcycle found" << std::endl;
		return;
	}
	if (_visited[index]._mark == mark)
	{
		
		std::vector<int> npath = path;
		npath.push_back(index);
		if (isCycle(npath))
		{
			//std::cout << "Cycle added" << mark << std::endl;
			addEndCycle(npath);
		}
		//_cyclepath.push_back(npath);
		
		

		
		return;
	}
	
	//std::cout << "Mark " << mark << " index " << index << ": " << path.size() << std::endl;
	_visited[index]._mark = mark;
	//_visited[index]._path = path;

	
	for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	{
		//std::cout << "Currently at " << index << ":" << i << " out of " << _ingraph[index]._providesFor.size() << std::endl;
		/*
		for (int j = 0; j < path.size(); ++j)
		{
			std::cout << path[j] << ' ';
		}
		std::cout << std::endl;
		*/

		std::vector<int> npath = path;
		npath.push_back(index);
		if (isCycle(npath))
		{
			addEndCycle(npath);
			//std::cout << "Exit recursion" << std::endl;
		}
		else
		{
			fire(_ingraph[index]._providesFor[i], mark, npath);	
		}
	}
	

	/*	
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
	*/

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

/*
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

	
	//for (int i = 0; i < _ingraph[index]._providesFor.size(); ++i)
	//{
	//	int stop = false;
	//	for (int j = 0; j < _visited[index]._path.size(); ++j)
	//	{
	//		if (_ingraph[index]._providesFor[i] == _visited[index]._path[j])
	//		{
	//			stop = true;
	//			ret.push_back(_ingraph[index]._providesFor[i]);
	//			break;
	//		}
	//	}

	//	if (stop)
	//	{
	//		break;
	//	}

	//}
	
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
*/

std::vector<int> CycleSearcher::extractCycle(std::vector<int> toExtract) const
{
	if (!isCycle(toExtract))
	{
		throw std::logic_error("CycleSearcher: is not a loop (broken algo)");
	}

	int loopHead = -1;
	for (int i = 0; i < toExtract.size() - 1; ++i)
	{
		if (toExtract[i] == toExtract[toExtract.size() - 1])
		{
			loopHead = i;
			break;
		}
	}

	if (loopHead == -1)
	{
		throw std::logic_error("CycleSearcher: is not a loop (broken isCycle)");
	}

	std::vector<int> ret(toExtract.begin() + loopHead, toExtract.end());

	return ret;
}

/*
CycleContainer CycleSearcher::getCycleContainer() const
{
	CycleContainer ret;

	for (int i = 0; i < _selfcycle.size(); ++i)
	{
		ret.addSelfCycle(_selfcycle[i]);
	}

	for (int i = 0; i < _cyclepath.size(); ++i)
	{
		//std::vector<int> toadd = rebuildCycle(_endcycle[i]);
		std::vector<int> toadd = extractCycle(_cyclepath[i]);
		//ret.addCycle(toadd);
		ret.addCycle(toadd);
		//std::cout << i << " out of " << _endcycle.size() << std::endl;
	}

	return ret;
}
*/

std::vector< std::vector<int> > CycleSearcher::getCycles() const
{
	std::vector< std::vector<int> > ret;

	for (int i = 0; i < _cyclepath.size(); ++i)
	{
		//std::vector<int> toadd = rebuildCycle(_endcycle[i]);
		std::vector<int> toadd = extractCycle(_cyclepath[i]);
		//ret.addCycle(toadd);
		ret.push_back(toadd);
		//std::cout << i << " out of " << _endcycle.size() << std::endl;
	}

	return ret;
}

std::vector<int> CycleSearcher::getSelfCycles() const
{
	return _selfcycle;
}

/*
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
*/