#include "graph.hpp"

//##################################################//
//													//
//					GraphNode						//
//													//
//##################################################//

//Nothing is needed anymore. GraphNode is now a subclass
//of Graph


//##################################################//
//													//
//						Graph						//
//													//
//##################################################//

Graph::Graph()
{
	//_ingraph = std::vector<Package>();
}

int Graph::getAmount() const
{
	return _ingraph.size();
}

void Graph::buildGraph(Container src)
{
	if (src.size() == 0)
		return;

	for (int i = 0; i < src.size(); ++i)
	{
		_ingraph.push_back(GraphNode(src[i]));
	}

	fillRequires();
	fillProvidesFor();
}


int Graph::findNodeProviding(std::string pkg) const
{
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		Package grpkg = _ingraph[i]._pkg;
		//std::cout << grpkg.provides(pkg) << std::endl;
		if (grpkg.provides(pkg))
		{
			//std::cout << "GRAPH PROVIDES TRUE:" << grpkg.getDesc() << " - " << pkg << std::endl;
			return i;
		}
	}

	return -1;
}

int Graph::findNodeRequiring(std::string pkg) const
{
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		if (_ingraph[i]._pkg.requires(pkg))
		{
			return i;
		}
	}

	return -1;
}

//TODO: Fix with exceptions or something, printing for debug right now
void Graph::fillRequires()
{
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		std::vector<std::string> reqs = _ingraph[i]._pkg.getRequires();
		for (int j = 0; j < reqs.size(); ++j)
		{
			int reqindex = findNodeProviding(reqs[j]);
			if (reqindex == -1)
			{
				_ingraph[i]._brokenDep = true;
				//std::string exmsg = "Graph: failed to find anyone providing <" + reqs[j] + "> for <" + _ingraph[i]._pkg.getName() + ">\n";
				//throw std::runtime_error(exmsg);
				std::cout << "Nobody provides <" << reqs[j] << "> for <" << _ingraph[i]._pkg.getName() << ">\n"; 
			}
			else
			{
				_ingraph[i]._requires.push_back(reqindex);
			}
			//sleep(1);
		}

		if (_ingraph[i]._requires.size() != _ingraph[i]._pkg.getReqAmount())
		{
			//throw std::runtime_error("One of the package requirements could not be resolved propely");
			std::cout << "Req " << i << ": something went horribly wrong" << std::endl;
		}
		else
		{
			std::cout << "Req " << i << ": All Good!" << std::endl;
		}
	}
}

//TODO: Fix with exceptions or something, printing for debug right now
void Graph::fillProvidesFor()
{
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		std::vector<std::string> provs = _ingraph[i]._pkg.getProvides();
		for (int j = 0; j < provs.size(); ++j)
		{
			int provindex = findNodeRequiring(provs[j]);
			if (provindex == -1)
			{
				//Nobody cares
				//std::cout << "Failed to find anyone who needs: " << provs[j] << std::endl;
			}
			else
			{
				_ingraph[i]._providesFor.push_back(provindex);
			}
		}
		std::cout << "Prov " << i << ": done\n";
	}
}


bool Graph::inGraph(const Package cmp) const
{
	for (int i = 0; i < _ingraph.size(); ++i)
	{
		if (cmp == _ingraph[i]._pkg)
		{
			return true;
		}
	}

	return false;
}

const Package& Graph::operator[] (int index) const
{
	if (index >= _ingraph.size())
	{
		throw std::logic_error("Graph: Tried to request index higher than amount of elements");
	}

	return _ingraph[index]._pkg;
}

void Graph::debugPrintNodeVectors(int index) const
{
	Graph::GraphNode tmp = _ingraph[index];
	int provsize = tmp._providesFor.size();
	int reqsize = tmp._requires.size();

	std::cout << "req:\n";

	for (int i = 0; i < reqsize; ++i)
	{
		std::cout << "\t " << tmp._requires[i] << std::endl;
	}

	std::cout << "prov:\n";

	for (int i = 0; i < provsize; ++i)
	{
		std::cout << "\t " << tmp._providesFor[i] << std::endl;
	}
}

std::string Graph::printInfo(int index) const
{
	if (index >= _ingraph.size())
	{
		throw std::logic_error("Graph: Tried to request index higher than amount of elements");
	}

	//debugPrintNodeVectors(index);

	Graph::GraphNode tmp = _ingraph[index];

	int provsize = tmp._providesFor.size();
	int reqsize = tmp._requires.size();

	std::string ret = "";
	if (tmp._brokenDep)
	{
		ret+= "!!! BROKEN DEPENDENCIES !!!\n\n";
	}
	ret += "PACKAGE NAME: " + tmp._pkg.getName() + "\n";
	ret += "PACKAGE DESCRIPTION: " + tmp._pkg.getDesc() + "\n";
	
	ret += "REQUIRES " + std::to_string(reqsize) + " PACKAGES\n";
	ret += "REQUIRES:\n";
	for (int i = 0; i < reqsize; ++i)
	{
		int pkgindex = _ingraph[index]._requires[i];
		ret += "\t* " + _ingraph[pkgindex]._pkg.getName() + '\n';
		ret += "\t-> " + _ingraph[pkgindex]._pkg.getDesc() + '\n';
	}

	ret += "PROVIDES FOR " + std::to_string(provsize) + " PACKAGES\n";	
	for (int i = 0; i < provsize; ++i)
	{
		ret += "\t* " + _ingraph[_ingraph[index]._providesFor[i]]._pkg.getName() + '\n';
		ret += "\t-> " + _ingraph[_ingraph[index]._providesFor[i]]._pkg.getDesc() + '\n';

	}
	
	return ret;
	
}